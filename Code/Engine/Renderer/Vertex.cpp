#include "Engine/Renderer/Vertex.hpp"
#include "Engine/Renderer/ShaderProgram.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Engine/Renderer/OpenGLExtensions.hpp"
#include "Engine/Renderer/Renderer.hpp"

size_t inPositionAttrib = std::hash<std::string>{}("inPosition");
size_t inColorAttrib = std::hash<std::string>{}("inColor");
size_t inUV0Attrib = std::hash<std::string>{}("inUV0");
size_t inTangentAttrib = std::hash<std::string>{}("inTangent");
size_t inBitangentAttrib = std::hash<std::string>{}("inBitangent");
size_t inNormalAttrib = std::hash<std::string>{}("inNormal");
size_t inNormalizedGlyphPositionAttrib = std::hash<std::string>{}("inNormalizedGlyphPosition");
size_t inNormalizedStringPositionAttrib = std::hash<std::string>{}("inNormalizedStringPosition");
size_t inBoneWeightsAttrib = std::hash<std::string>{}("inBoneWeights");
size_t inBoneIndicesAttrib = std::hash<std::string>{}("inBoneIndices");
size_t inFloatData0Attrib = std::hash<std::string>{}("inFloatData0");


//Defaults for the vertex master's uninitialized values
//-----------------------------------------------------------------------------------
Vertex_Master::Vertex_Master()
    : position(Vector3::ZERO)
    , tangent(Vector3::ZERO)
    , bitangent(Vector3::ZERO)
    , normal(Vector3::ZERO)
    , color(RGBA::WHITE)
    , uv0(Vector2::ZERO)
    , uv1(Vector2::ZERO)
    , normalizedGlyphPosition(Vector2::ZERO)
    , normalizedStringPosition(Vector2::ZERO)
    , normalizedFragPosition(0.0f)
    , boneWeights(Vector4::UNIT_X)
    , boneIndices(Vector4Int::ZERO)
    , floatData0(Vector4::ZERO)
{

}

//-----------------------------------------------------------------------------------
void Vertex_PCUTB::Copy(const Vertex_Master& source, byte* destination)
{
    Vertex_PCUTB* pctub = (Vertex_PCUTB*)(destination);
    pctub->pos = source.position;
    pctub->color = source.color;
    pctub->texCoords = source.uv0;
    pctub->tangent = source.tangent;
    pctub->bitangent = source.bitangent;
}

//-----------------------------------------------------------------------------------
void Vertex_PCT::Copy(const Vertex_Master& source, byte* destination)
{
    Vertex_PCT* pct = (Vertex_PCT*)(destination);
    pct->pos = source.position;
    pct->color = source.color;
    pct->texCoords = source.uv0;
}

//-----------------------------------------------------------------------------------
void Vertex_PCT::BindMeshToVAO(GLuint vao, GLuint vbo, GLuint ibo, ShaderProgram* program)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    program->ShaderProgramBindProperty(inPositionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_PCT), offsetof(Vertex_PCT, pos));
    program->ShaderProgramBindProperty(inColorAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex_PCT), offsetof(Vertex_PCT, color));
    program->ShaderProgramBindProperty(inUV0Attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_PCT), offsetof(Vertex_PCT, texCoords));
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    if (ibo != NULL)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    }
    glBindVertexArray(NULL);
}

//-----------------------------------------------------------------------------------
void Vertex_PCUTB::BindMeshToVAO(GLuint vao, GLuint vbo, GLuint ibo, ShaderProgram* program)
{
    glBindVertexArray(vao);
    GL_CHECK_ERROR();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GL_CHECK_ERROR();
    program->ShaderProgramBindProperty(inPositionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_PCUTB), offsetof(Vertex_PCUTB, pos));
    program->ShaderProgramBindProperty(inColorAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex_PCUTB), offsetof(Vertex_PCUTB, color));
    program->ShaderProgramBindProperty(inUV0Attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_PCUTB), offsetof(Vertex_PCUTB, texCoords));
    program->ShaderProgramBindProperty(inTangentAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_PCUTB), offsetof(Vertex_PCUTB, tangent));
    program->ShaderProgramBindProperty(inBitangentAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_PCUTB), offsetof(Vertex_PCUTB, bitangent));
    GL_CHECK_ERROR();
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    GL_CHECK_ERROR();
    if (ibo != NULL)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        GL_CHECK_ERROR();
    }
    glBindVertexArray(NULL);
    GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------
void Vertex_TextPCT::Copy(const Vertex_Master& source, byte* destination)
{
    Vertex_TextPCT* textPCT = (Vertex_TextPCT*)(destination);
    textPCT->pos = source.position;
    textPCT->color = source.color;
    textPCT->texCoords = source.uv0;
    textPCT->normalizedGlyphPosition = source.normalizedGlyphPosition;
    textPCT->normalizedStringPosition = source.normalizedStringPosition;
}

//-----------------------------------------------------------------------------------
void Vertex_TextPCT::BindMeshToVAO(GLuint vao, GLuint vbo, GLuint ibo, ShaderProgram* program)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    program->ShaderProgramBindProperty(inPositionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_TextPCT), offsetof(Vertex_TextPCT, pos));
    program->ShaderProgramBindProperty(inColorAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex_TextPCT), offsetof(Vertex_TextPCT, color));
    program->ShaderProgramBindProperty(inUV0Attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_TextPCT), offsetof(Vertex_TextPCT, texCoords));
    program->ShaderProgramBindProperty(inNormalizedGlyphPositionAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_TextPCT), offsetof(Vertex_TextPCT, normalizedGlyphPosition));
    program->ShaderProgramBindProperty(inNormalizedStringPositionAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_TextPCT), offsetof(Vertex_TextPCT, normalizedStringPosition));
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    if (ibo != NULL)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    }
    glBindVertexArray(NULL);
}

//-----------------------------------------------------------------------------------
void Vertex_SkinnedPCTN::Copy(const Vertex_Master& source, byte* destination)
{
    Vertex_SkinnedPCTN* skinnedPCT = (Vertex_SkinnedPCTN*)(destination);
    skinnedPCT->pos = source.position;
    skinnedPCT->color = source.color;
    skinnedPCT->texCoords = source.uv0;
    skinnedPCT->normal = source.normal;
    skinnedPCT->boneIndices = source.boneIndices;
    skinnedPCT->boneWeights = source.boneWeights;
}

//-----------------------------------------------------------------------------------
void Vertex_SkinnedPCTN::BindMeshToVAO(GLuint vao, GLuint vbo, GLuint ibo, ShaderProgram* program)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    program->ShaderProgramBindProperty(inPositionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_SkinnedPCTN), offsetof(Vertex_SkinnedPCTN, pos));
    program->ShaderProgramBindProperty(inColorAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex_SkinnedPCTN), offsetof(Vertex_SkinnedPCTN, color));
    program->ShaderProgramBindProperty(inUV0Attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_SkinnedPCTN), offsetof(Vertex_SkinnedPCTN, texCoords));
    program->ShaderProgramBindProperty(inNormalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_SkinnedPCTN), offsetof(Vertex_SkinnedPCTN, normal));
    program->ShaderProgramBindIntegerProperty(inBoneIndicesAttrib, 4, GL_INT, sizeof(Vertex_SkinnedPCTN), offsetof(Vertex_SkinnedPCTN, boneIndices));
    program->ShaderProgramBindProperty(inBoneWeightsAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_SkinnedPCTN), offsetof(Vertex_SkinnedPCTN, boneWeights));
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    if (ibo != NULL)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    }
    glBindVertexArray(NULL);
}

//-----------------------------------------------------------------------------------
void Vertex_PCTD::Copy(const Vertex_Master& source, byte* destination)
{
    Vertex_PCTD* pctd = (Vertex_PCTD*)(destination);
    pctd->pos = source.position;
    pctd->color = source.color;
    pctd->texCoords = source.uv0;
    pctd->floatData0 = source.floatData0;
}

//-----------------------------------------------------------------------------------
void Vertex_PCTD::BindMeshToVAO(GLuint vao, GLuint vbo, GLuint ibo, ShaderProgram* program)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    program->ShaderProgramBindProperty(inPositionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_PCTD), offsetof(Vertex_PCTD, pos));
    program->ShaderProgramBindProperty(inColorAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex_PCTD), offsetof(Vertex_PCTD, color));
    program->ShaderProgramBindProperty(inUV0Attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_PCTD), offsetof(Vertex_PCTD, texCoords));
    program->ShaderProgramBindProperty(inFloatData0Attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PCTD), offsetof(Vertex_PCTD, floatData0));
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    if (ibo != NULL)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    }
    glBindVertexArray(NULL);
}

//-----------------------------------------------------------------------------------
void Vertex_Sprite::Copy(const Vertex_Master& source, byte* destination)
{
    Vertex_Sprite* spriteVert = (Vertex_Sprite*)(destination);
    spriteVert->position = Vector2(source.position.x, source.position.y);
    spriteVert->color = source.color;
    spriteVert->uv = source.uv0;
}

//-----------------------------------------------------------------------------------
void Vertex_Sprite::BindMeshToVAO(GLuint vao, GLuint vbo, GLuint ibo, ShaderProgram* program)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    program->ShaderProgramBindProperty(inPositionAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Sprite), offsetof(Vertex_Sprite, position));
    program->ShaderProgramBindProperty(inColorAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex_Sprite), offsetof(Vertex_Sprite, color));
    program->ShaderProgramBindProperty(inUV0Attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Sprite), offsetof(Vertex_Sprite, uv));
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    if (ibo != NULL)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    }
    glBindVertexArray(NULL);
}