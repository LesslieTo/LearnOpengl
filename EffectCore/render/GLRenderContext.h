#ifndef GLRENDERCONTEXT_H
#define GLRENDERCONTEXT_H

#include "sample/GLSampleBase.h"
class GLRenderContext
{
public:
    GLRenderContext();
    ~GLRenderContext();

public:
    void SetImagePath(const std::string &path);

    void SetImageData(int format, int width, int height, uint8_t* pData);

    void SetImageDataWithIndex(int index, int format, int width, int height, uint8_t* pData);

    void SetParamsInt(int paramType, int value0, int value1);

    void SetParamsFloat(int paramType, float value0, float value1);

    void SetParamsShortArr(short *const pShortArr, int arrSize);

    void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    void OnSurfaceCreated();

    void OnSurfaceChanged(int width, int height);

    void OnDrawFrame();

    static GLRenderContext* GetInstance();
    static void DestroyInstance();

private:
    static GLRenderContext *m_pContext;
    GLSampleBase *m_pBeforeSample;
    GLSampleBase *m_pCurSample {nullptr};
    int m_ScreenW {0};
    int m_ScreenH {0};
};

#endif // GLRENDERCONTEXT_H
