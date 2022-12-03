#ifndef GLSAMPLEBASE_H
#define GLSAMPLEBASE_H
#include "ImageDef.h"
#include "Lock.h"

#define SAMPLE_TYPE                             200
#define SAMPLE_TYPE_KEY_TRIANGLE                SAMPLE_TYPE + 0
#define SAMPLE_TYPE_KEY_TEXTURE_MAP             SAMPLE_TYPE + 1
#define SAMPLE_TYPE_KEY_YUV_TEXTURE_MAP         SAMPLE_TYPE + 2
#define SAMPLE_TYPE_KEY_VAO                     SAMPLE_TYPE + 3
#define SAMPLE_TYPE_KEY_FBO                     SAMPLE_TYPE + 4

class GLSampleBase
{
public:
    GLSampleBase(){};
    virtual ~GLSampleBase(){};

    virtual void LoadImage(NativeImage* pImage)
    {}

    virtual void LoadMultiImageWithIndex(int index, NativeImage *pImage)
    {}

    virtual void LoadShortArrData(short* const pShortArr, int arrSize)
    {}

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY)
    {}

    virtual void SetTouchLocation(float x, float y)
    {}

    virtual void SetGravityXY(float x, float y)
    {}

    virtual void Init() = 0;
    virtual void Draw(int screenW, int screenH) = 0;
    virtual void Destroy() = 0;


protected:
    uint32_t m_VertexShader {0};
    uint32_t m_FragmentShader {0};
    uint32_t m_ProgramObj {0};
    MySyncLock m_Lock;
    int m_SurfaceWidth {0};
    int m_SurfaceHeight {0};
};

#endif // GLSAMPLEBASE_H
