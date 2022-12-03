#ifndef FRAMEBUFFERSAMPLE_H
#define FRAMEBUFFERSAMPLE_H

#include "GLSampleBase.h"
#include "util/ImageDef.h"
class FrameBufferSample : public GLSampleBase
{
public:
    FrameBufferSample();
    virtual ~FrameBufferSample();

    virtual void Init();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    void CreateFbo();

private:
    NativeImage m_RenderImage;
    uint32_t m_ProgramObj;
    uint32_t m_VertexShader;
    uint32_t m_FragmentShader;


    uint32_t m_SamplerLoc;
    uint32_t m_FboProgramObj ;
    uint32_t m_FboVertexShader;
    uint32_t m_FboFragmentShader;
    uint32_t m_FboSamplerLoc;

    uint32_t m_VBO = 0;
    uint32_t m_VAO = 0;
    uint32_t m_EBO = 0;
    uint32_t m_texture = 0;

    uint32_t m_frameBuffer ;
    uint32_t m_RBO ;
    uint32_t m_fVBO ;
    uint32_t m_fVAO;
    uint32_t m_textureFBO;
};

#endif // FRAMEBUFFERSAMPLE_H
