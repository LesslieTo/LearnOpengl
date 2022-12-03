#ifndef VAOSAMPLE_H
#define VAOSAMPLE_H


#include "GLSampleBase.h"
class VaoSample : public GLSampleBase
{
public:
    VaoSample();
    virtual ~VaoSample();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();
    virtual void Draw(int screenW, int screenH);
    virtual void Destroy();

private:
    uint32_t  m_VaoId;
    uint32_t  m_VboIds[2];
};

#endif // VAOSAMPLE_H
