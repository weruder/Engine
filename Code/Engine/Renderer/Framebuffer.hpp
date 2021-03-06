#pragma once
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Texture.hpp"
#include <vector>

class Framebuffer
{
    typedef unsigned int GLuint;
public:
    //CONSTRUCTORS//////////////////////////////////////////////////////////////////////////
    static Framebuffer* FramebufferCreate(size_t colorCount, Texture** colorTargets, Texture* depthStencilTarget);
    static void FramebufferDelete(Framebuffer *fbo);
    void AddColorTarget(Texture* colorTarget); //Adds the color target by pushing back on the list of color targets.
    Texture* SwapColorTarget(Texture* colorTarget, int index);
    void SwapColorTargetWithFBO(Framebuffer* otherFBO, int index);
    void FlushColorTargets();
    void ClearColorBuffer(int bufferNumber, const RGBA& clearColor);
    void Bind();
    void Unbind();
    void ClearDepthBuffer();
    void SwapColorTargets(unsigned int firstIndex = 0, unsigned int secondIndex = 1);

    //MEMBER VARIABLES/////////////////////////////////////////////////////////////////////////
    GLuint m_fboHandle;
    std::vector<Texture*> m_colorTargets;
    Texture* m_depthStencilTarget;
    size_t m_colorCount;
    uint32_t m_pixelWidth;
    uint32_t m_pixelHeight;
};