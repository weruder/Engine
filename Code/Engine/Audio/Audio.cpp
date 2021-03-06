//---------------------------------------------------------------------------
#include "Engine/Audio/Audio.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/Console.hpp"
#include "../Core/StringUtils.hpp"

AudioSystem* AudioSystem::instance = nullptr;

//-----------------------------------------------------------------------------------
CONSOLE_COMMAND(playsound)
{
    if (!args.HasArgs(1))
    {
        Console::instance->PrintLine("playsound <filename>", RGBA::RED);
        return;
    }
    std::string filepath = args.GetStringArgument(0);
    SoundID song = AudioSystem::instance->CreateOrGetSound(filepath);
    if (song == MISSING_SOUND_ID)
    {
        Console::instance->PrintLine("Could not find file.", RGBA::RED);
        return;
    }
    AudioSystem::instance->PlaySound(song);
}

//---------------------------------------------------------------------------
AudioSystem::AudioSystem()
    : m_fmodSystem( nullptr )
{
    InitializeFMOD();
}

//---------------------------------------------------------------------------
// FMOD startup code based on "GETTING STARTED With FMOD Ex Programmerís API for Windows" document
//	from the FMOD programming API at http://www.fmod.org/download/
//
void AudioSystem::InitializeFMOD()
{
    const int MAX_AUDIO_DEVICE_NAME_LEN = 256;
    FMOD_RESULT result;
    unsigned int fmodVersion;
    int numDrivers;
    FMOD_SPEAKERMODE speakerMode;
    FMOD_CAPS deviceCapabilities;
    char audioDeviceName[ MAX_AUDIO_DEVICE_NAME_LEN ];

    // Create a System object and initialize.
    result = FMOD::System_Create( &m_fmodSystem );
    ValidateResult( result );

    result = m_fmodSystem->getVersion( &fmodVersion );
    ValidateResult( result );

    if( fmodVersion < FMOD_VERSION )
    {
        DebuggerPrintf( "AUDIO SYSTEM ERROR!  Your FMOD .dll is of an older version (0x%08x == %d) than that the .lib used to compile this code (0x%08x == %d).\n", fmodVersion, fmodVersion, FMOD_VERSION, FMOD_VERSION );
    }

    result = m_fmodSystem->getNumDrivers( &numDrivers );
    ValidateResult( result );

    if( numDrivers == 0 )
    {
        result = m_fmodSystem->setOutput( FMOD_OUTPUTTYPE_NOSOUND );
        ValidateResult( result );
    }
    else
    {
        result = m_fmodSystem->getDriverCaps( 0, &deviceCapabilities, 0, &speakerMode );
        ValidateResult( result );

        // Set the user selected speaker mode.
        result = m_fmodSystem->setSpeakerMode( speakerMode );
        ValidateResult( result );

        if( deviceCapabilities & FMOD_CAPS_HARDWARE_EMULATED )
        {
            // The user has the 'Acceleration' slider set to off! This is really bad
            // for latency! You might want to warn the user about this.
            result = m_fmodSystem->setDSPBufferSize( 1024, 10 );
            ValidateResult( result );
        }

        result = m_fmodSystem->getDriverInfo( 0, audioDeviceName, MAX_AUDIO_DEVICE_NAME_LEN, 0 );
        ValidateResult( result );

        if( strstr( audioDeviceName, "SigmaTel" ) )
        {
            // Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
            // PCM floating point output seems to solve it.
            result = m_fmodSystem->setSoftwareFormat( 48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR );
            ValidateResult( result );
        }
    }

    result = m_fmodSystem->init( 100, FMOD_INIT_NORMAL, 0 );
    if( result == FMOD_ERR_OUTPUT_CREATEBUFFER )
    {
        // Ok, the speaker mode selected isn't supported by this sound card. Switch it
        // back to stereo...
        result = m_fmodSystem->setSpeakerMode( FMOD_SPEAKERMODE_STEREO );
        ValidateResult( result );

        // ... and re-init.
        result = m_fmodSystem->init( 100, FMOD_INIT_NORMAL, 0 );
        ValidateResult( result );
    }
}

//---------------------------------------------------------------------------
AudioSystem::~AudioSystem()
{
// 	FMOD_RESULT result = FMOD_OK;
// 	result = FMOD_System_Close( m_fmodSystem );
// 	result = FMOD_System_Release( m_fmodSystem );
// 	m_fmodSystem = nullptr;
}

//---------------------------------------------------------------------------
void AudioSystem::StopChannel(AudioChannelHandle channel)
{
    if (channel != nullptr)
    {
        FMOD::Channel* fmodChannel = (FMOD::Channel*) channel;
        fmodChannel->stop();
    }
}

//---------------------------------------------------------------------------
void AudioSystem::StopSound(SoundID soundID)
{
    AudioChannelHandle channel = m_channels[soundID];
    if (channel != nullptr)
    {
        FMOD::Channel* fmodChannel = (FMOD::Channel*) channel;
        fmodChannel->stop();
    }
}

//-----------------------------------------------------------------------------------
void AudioSystem::MultiplyCurrentFrequency(SoundID soundID, float multiplier)
{
    SetFrequency(soundID, GetFrequency(soundID) * multiplier);
}

//-----------------------------------------------------------------------------------
void AudioSystem::SetFrequency(SoundID soundID, float frequency)
{
    AudioChannelHandle channel = m_channels[soundID];
    if (channel != nullptr)
    {
        FMOD::Channel* fmodChannel = (FMOD::Channel*) channel;
        fmodChannel->setFrequency(frequency);
    }
}

//-----------------------------------------------------------------------------------
void AudioSystem::SetFrequency(AudioChannelHandle channel, float frequency)
{
    if (channel != nullptr)
    {
        FMOD::Channel* fmodChannel = (FMOD::Channel*) channel;
        fmodChannel->setFrequency(frequency);
    }
}

//-----------------------------------------------------------------------------------
float AudioSystem::GetVolume(AudioChannelHandle channel)
{
    float volume0to1 = -1.0f;
    ((FMOD::Channel*)channel)->getVolume(&volume0to1);
    return volume0to1;
}


//-----------------------------------------------------------------------------------
void AudioSystem::SetVolume(AudioChannelHandle channel, float volume0to1)
{
    ((FMOD::Channel*)channel)->setVolume(volume0to1);
}

//-----------------------------------------------------------------------------------
float AudioSystem::GetFrequency(SoundID soundID)
{
    float frequency = -1.0f;
    AudioChannelHandle channel = m_channels[soundID];
    if (channel != nullptr)
    {
        FMOD::Channel* fmodChannel = (FMOD::Channel*) channel;
        fmodChannel->getFrequency(&frequency);
    }
    return frequency;
}

//-----------------------------------------------------------------------------------
void AudioSystem::SetMIDISpeed(SoundID soundID, float speedMultiplier)
{
    FMOD::Sound* sound = m_registeredSounds[soundID];
    if (!sound)
    {
        return;
    }

    sound->setMusicSpeed(speedMultiplier);
}

//-----------------------------------------------------------------------------------
void AudioSystem::SetMIDISpeed(RawSoundHandle songHandle, float speedMultiplier)
{
    FMOD::Sound* sound = static_cast<FMOD::Sound*>(songHandle);
    if (!sound)
    {
        return;
    }

    sound->setMusicSpeed(speedMultiplier);
}

//-----------------------------------------------------------------------------------
void AudioSystem::ReleaseRawSong(RawSoundHandle songHandle)
{
    ASSERT_OR_DIE(static_cast<FMOD::Sound*>(songHandle)->release() == FMOD_OK, "Failed to release a song.");
}

//---------------------------------------------------------------------------
SoundID AudioSystem::CreateOrGetSound( const std::string& soundFileName )
{
    std::map<std::string, SoundID>::iterator found = m_registeredSoundIDs.find( soundFileName );
    if( found != m_registeredSoundIDs.end() )
    {
        return found->second;
    }
    else
    {
        FMOD::Sound* newSound = nullptr;
        m_fmodSystem->createSound( soundFileName.c_str(), FMOD_DEFAULT, nullptr, &newSound );
        if( newSound )
        {
            SoundID newSoundID = m_registeredSounds.size();
            m_registeredSoundIDs[ soundFileName ] = newSoundID;
            m_registeredSounds.push_back( newSound );
            return newSoundID;
        }
    }

    return MISSING_SOUND_ID;
}

//-----------------------------------------------------------------------------------
SoundID AudioSystem::CreateOrGetSound(const std::wstring& wideSoundFileName)
{
    char fileName[MAX_PATH * 8];
    WideCharToMultiByte(CP_UTF8, 0, wideSoundFileName.c_str(), -1, fileName, sizeof(fileName), NULL, NULL);

    std::string soundFileName(fileName);
    std::map<std::string, SoundID>::iterator found = m_registeredSoundIDs.find(soundFileName);
    if (found != m_registeredSoundIDs.end())
    {
        return found->second;
    }
    else
    {
        FMOD::Sound* newSound = nullptr;
        m_fmodSystem->createSound((char*)wideSoundFileName.c_str(), FMOD_DEFAULT | FMOD_UNICODE, nullptr, &newSound);
        if (newSound)
        {
            SoundID newSoundID = m_registeredSounds.size();
            m_registeredSoundIDs[soundFileName] = newSoundID;
            m_registeredSounds.push_back(newSound);
            return newSoundID;
        }
    }

    return MISSING_SOUND_ID;
}

//---------------------------------------------------------------------------
void AudioSystem::PlaySound( SoundID soundID, float volumeLevel )
{
    unsigned int numSounds = m_registeredSounds.size();
    if( soundID < 0 || soundID >= numSounds )
        return;

    FMOD::Sound* sound = m_registeredSounds[ soundID ];
    if( !sound )
        return;

    FMOD::Channel* channelAssignedToSound = nullptr;
    m_fmodSystem->playSound( FMOD_CHANNEL_FREE, sound, false, &channelAssignedToSound );
    if( channelAssignedToSound )
    {
        channelAssignedToSound->setVolume(volumeLevel);
    }
    m_channels[soundID] = channelAssignedToSound;
}

//-----------------------------------------------------------------------------------
void AudioSystem::PlayLoopingSound(SoundID soundID, float volumeLevel)
{
    PlaySound(soundID, volumeLevel);
    FMOD::Channel* channelAssignedToSound = static_cast<FMOD::Channel*>(m_channels[soundID]);
    channelAssignedToSound->setMode(FMOD_LOOP_NORMAL);
}

//-----------------------------------------------------------------------------------
void AudioSystem::SetLooping(SoundID soundID, bool isLooping)
{
    FMOD::Channel* channelAssignedToSound = static_cast<FMOD::Channel*>(m_channels[soundID]);
    channelAssignedToSound->setMode(isLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
}

//---------------------------------------------------------------------------
void AudioSystem::Update( float deltaSeconds )
{
    FMOD_RESULT result = m_fmodSystem->update();
    ValidateResult( result );
    //Unused
    (void)(deltaSeconds);
}

//---------------------------------------------------------------------------
void AudioSystem::ValidateResult(FMOD_RESULT result)
{
    if( result != FMOD_OK )
    {
        DebuggerPrintf( "AUDIO SYSTEM ERROR: Got error result code %d.\n", result );
        __debugbreak();
    }
}

//-----------------------------------------------------------------------------------
bool AudioSystem::IsPlaying(AudioChannelHandle channel)
{
    if (!channel)
    {
        return false;
    }

    bool isPlaying = false;
    FMOD::Channel* channelAssignedToSound = static_cast<FMOD::Channel*>(channel);
    channelAssignedToSound->isPlaying(&isPlaying);
    return isPlaying;
}

//-----------------------------------------------------------------------------------
unsigned int AudioSystem::GetPlaybackPositionMS(AudioChannelHandle channel)
{
    unsigned int outTimestampMS = 0;
    ASSERT_OR_DIE(channel, "Channel passed to GetPlaybackPositionMS was null.");

    FMOD::Channel* channelAssignedToSound = static_cast<FMOD::Channel*>(channel);
    channelAssignedToSound->getPosition(&outTimestampMS, FMOD_TIMEUNIT_MS);
    return outTimestampMS;
}

//-----------------------------------------------------------------------------------
void AudioSystem::SetPlaybackPositionMS(AudioChannelHandle channel, unsigned int timestampMS)
{
    ASSERT_OR_DIE(channel, "Channel passed to SetPlaybackPositionMS was null.");

    FMOD::Channel* channelAssignedToSound = static_cast<FMOD::Channel*>(channel);
    channelAssignedToSound->setPosition(timestampMS, FMOD_TIMEUNIT_MS);
}

//-----------------------------------------------------------------------------------
unsigned int AudioSystem::GetSoundLengthMS(SoundID soundHandle)
{
    unsigned int outSoundLengthMS = 0;
    FMOD::Sound* sound = m_registeredSounds[soundHandle];

    sound->getLength(&outSoundLengthMS, FMOD_TIMEUNIT_MS);

    return outSoundLengthMS;
}

//-----------------------------------------------------------------------------------
unsigned int AudioSystem::GetSoundLengthMS(RawSoundHandle songHandle)
{
    unsigned int outSoundLengthMS = 0;
    FMOD::Sound* sound = static_cast<FMOD::Sound*>(songHandle);

    sound->getLength(&outSoundLengthMS, FMOD_TIMEUNIT_MS);

    return outSoundLengthMS;
}

//-----------------------------------------------------------------------------------
AudioChannelHandle AudioSystem::GetChannel(SoundID songHandle)
{
    AudioChannelHandle channelHandle = nullptr;

    auto foundChannel = m_channels.find(songHandle);
    if (foundChannel != m_channels.end())
    {
        channelHandle = (*foundChannel).second;
    }

    return channelHandle;
}

//-----------------------------------------------------------------------------------
RawSoundHandle AudioSystem::LoadRawSound(const std::wstring& wideSoundFileName, unsigned int& errorValue)
{
    char fileName[MAX_PATH * 8];
    WideCharToMultiByte(CP_UTF8, 0, wideSoundFileName.c_str(), -1, fileName, sizeof(fileName), NULL, NULL);

    FMOD::Sound* newSound = nullptr;
    errorValue = static_cast<unsigned int>(m_fmodSystem->createSound((char*)wideSoundFileName.c_str(), FMOD_DEFAULT | FMOD_UNICODE, nullptr, &newSound));
    return newSound;
}

//-----------------------------------------------------------------------------------
AudioChannelHandle AudioSystem::PlayRawSong(RawSoundHandle songHandle, float volumeLevel /*= 1.f*/)
{
    FMOD::Sound* sound = (FMOD::Sound*)songHandle;
    ASSERT_OR_DIE(sound, "Couldn't play the song handle from PlayRawSong");

    FMOD::Channel* channelAssignedToSound = nullptr;
    m_fmodSystem->playSound(FMOD_CHANNEL_FREE, sound, false, &channelAssignedToSound);
    if (channelAssignedToSound)
    {
        channelAssignedToSound->setVolume(volumeLevel);
    }
    return channelAssignedToSound;
}

//-----------------------------------------------------------------------------------
void AudioSystem::SetLooping(AudioChannelHandle rawSongChannel, bool isLooping)
{
    FMOD::Channel* channelAssignedToSound = static_cast<FMOD::Channel*>(rawSongChannel);
    channelAssignedToSound->setMode(isLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
}

//-----------------------------------------------------------------------------------
void AudioSystem::CreateDSPByType(FMOD_DSP_TYPE type, DSPHandle** dsp)
{
    bool success = true;
    FMOD_RESULT result = AudioSystem::instance->m_fmodSystem->createDSPByType(type, dsp);
    if (result)
    {
        success = false;
    }

    ASSERT_RECOVERABLE(success, "Couldn't create DSP");
}