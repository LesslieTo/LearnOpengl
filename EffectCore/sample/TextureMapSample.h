#ifndef TEXTUREMAPSAMPLE_H
#define TEXTUREMAPSAMPLE_H

#include "GLSampleBase.h"
#include "util/ImageDef.h"

class TextureMapSample : public GLSampleBase
{
public:
    TextureMapSample();
    virtual ~TextureMapSample();

    void LoadImage(NativeImage* pImage);

    virtual void Init();

    virtual void Draw(int sceenW, int sceenH);

    virtual void Destroy();

private:
    uint32_t m_TextureId {0};
    uint32_t  m_SamplerLoc {0};
    NativeImage m_RenderImage;

    uint32_t m_VaoIds[1] = { 0 };
    uint32_t m_VboIds[3] = { 0 };
};

#endif // TEXTUREMAPSAMPLE_H
