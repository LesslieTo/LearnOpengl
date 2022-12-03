#ifndef FBOSAMPLE_H
#define FBOSAMPLE_H


#include "GLSampleBase.h"
#include "util/ImageDef.h"
class FBOSample : public GLSampleBase
{
public:
    FBOSample();
    virtual ~FBOSample();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();
    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    bool CreateFrameBufferObj();

    bool CreateFrameBufferObj_();

private:
    uint32_t m_ImageTextureId { 0 };
    uint32_t m_FboTextureId { 0 };
    uint32_t m_RboId { 0 };
    uint32_t m_depthId { 0 };
    uint32_t m_FboId;
    uint32_t m_VaoIds[2] { 0 };
    uint32_t m_VboIds[4] { 0 };
    uint32_t m_SamplerLoc;
    uint32_t m_FboProgramObj ;
    uint32_t m_FboVertexShader;
    uint32_t m_FboFragmentShader;
    uint32_t m_FboSamplerLoc;

    NativeImage m_RenderImage;
};

#endif // FBOSAMPLE_H
