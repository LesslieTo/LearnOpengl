#define STB_IMAGE_IMPLEMENTATION
#include "GLRenderContext.h"
#include "external/glad/glad.h"
#include "external/image/stb_image.h"
#include "sample/TriangleSample.h"
#include "sample/TextureMapSample.h"
#include "sample/FrameBufferSample.h"
#include "sample/VaoSample.h"
#include "sample/FBOSample.h"
#include <iostream>

GLRenderContext* GLRenderContext::m_pContext = nullptr;
GLRenderContext::GLRenderContext()
{
    m_pCurSample = new TriangleSample();
    m_pBeforeSample = nullptr;
}

GLRenderContext::~GLRenderContext()
{
    if (m_pCurSample)
    {
        m_pCurSample->Destroy();
        delete m_pCurSample;
        m_pCurSample = nullptr;
    }

    if (m_pBeforeSample)
    {
        m_pBeforeSample->Destroy();
        delete m_pBeforeSample;
        m_pBeforeSample = nullptr;
    }
}

void GLRenderContext::SetParamsInt(int paramType, int value0, int value1)
{
    if (paramType == SAMPLE_TYPE)
    {
        m_pBeforeSample = m_pCurSample;

        //LOGCATE("MyGLRenderContext::SetParamsInt 0 m_pBeforeSample = %p", m_pBeforeSample);
        switch (value0)
        {
        case SAMPLE_TYPE_KEY_TRIANGLE:
            m_pCurSample = new TriangleSample();
            m_pCurSample->Init();
            break;
        case SAMPLE_TYPE_KEY_TEXTURE_MAP:
            m_pCurSample = new TextureMapSample();
            m_pCurSample->Init();
            break;
        case SAMPLE_TYPE_KEY_VAO:
            m_pCurSample = new VaoSample();
            m_pCurSample->Init();
            break;
        case SAMPLE_TYPE_KEY_FBO:
            //m_pCurSample = new FBOSample();
             m_pCurSample = new FrameBufferSample();
            SetImagePath("/Users/yafei.du/Desktop/Console/container.jpeg");
            m_pCurSample->Init();
            break;
        default:
            m_pCurSample = nullptr;
            break;
        }
    }
}

void GLRenderContext::SetParamsFloat(int paramType, float value0, float value1) {
    //LOGCATE("MyGLRenderContext::SetParamsFloat paramType=%d, value0=%f, value1=%f", paramType, value0, value1);
    if(m_pCurSample)
    {
        switch (paramType)
        {
        //case SAMPLE_TYPE_KEY_SET_TOUCH_LOC:
        m_pCurSample->SetTouchLocation(value0, value1);
        break;
        //case SAMPLE_TYPE_SET_GRAVITY_XY:
        m_pCurSample->SetGravityXY(value0, value1);
        break;
        default:
            break;

        }
    }
}

void GLRenderContext::SetParamsShortArr(short *const pShortArr, int arrSize)
{
    if(m_pCurSample){
        m_pCurSample->LoadShortArrData(pShortArr, arrSize);
    }
}

void GLRenderContext::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY)
{
    //LOGCATE("MyGLRenderContext::UpdateTransformMatrix [rotateX, rotateY, scaleX, scaleY] = [%f, %f, %f, %f]", rotateX, rotateY, scaleX, scaleY);
    if (m_pCurSample)
    {
        m_pCurSample->UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);
    }
}

void GLRenderContext::SetImageDataWithIndex(int index, int format, int width, int height, uint8_t *pData)
{
    //LOGCATE("MyGLRenderContext::SetImageDataWithIndex index=%d, format=%d, width=%d, height=%d, pData=%p", index, format, width, height, pData);
    NativeImage nativeImage;
    nativeImage.format = format;
    nativeImage.width = width;
    nativeImage.height = height;
    nativeImage.ppPlane[0] = pData;

    switch (format)
    {
    case IMAGE_FORMAT_NV12:
    case IMAGE_FORMAT_NV21:
        nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
        break;
    case IMAGE_FORMAT_I420:
        nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
        nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
        break;
    default:
        break;
    }

    if (m_pCurSample)
    {
        m_pCurSample->LoadMultiImageWithIndex(index, &nativeImage);
    }
}

void GLRenderContext::SetImagePath(const std::string &path)
{
    int width, height, channel;
    auto data = stbi_load(path.c_str(), &width, &height, &channel, 0);
    if(data){
         std::cout << "Image read success !" << std::endl;
         SetImageData(IMAGE_FORMAT_P010, width, height, data);
    }
    stbi_image_free(data);
}

void GLRenderContext::SetImageData(int format, int width, int height, uint8_t *pData)
{
    //LOGCATE("MyGLRenderContext::SetImageData format=%d, width=%d, height=%d, pData=%p", format, width, height, pData);
    NativeImage nativeImage;
    nativeImage.format = format;
    nativeImage.width = width;
    nativeImage.height = height;
    nativeImage.ppPlane[0] = pData;

    switch (format)
    {
    case IMAGE_FORMAT_NV12:
    case IMAGE_FORMAT_NV21:
        nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
        break;
    case IMAGE_FORMAT_I420:
        nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
        nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
        break;
    default:
        break;
    }

    if (m_pCurSample)
    {
        m_pCurSample->LoadImage(&nativeImage);
    }
}

void GLRenderContext::OnSurfaceCreated()
{
    //LOGCATE("MyGLRenderContext::OnSurfaceCreated");
    glClearColor(1.0f,1.0f,1.0f, 1.0f);
}

void GLRenderContext::OnSurfaceChanged(int width, int height)
{
    //LOGCATE("MyGLRenderContext::OnSurfaceChanged [w, h] = [%d, %d]", width, height);
    glViewport(0, 0, width, height);
    m_ScreenW = width;
    m_ScreenH = height;
}

void GLRenderContext::OnDrawFrame()
{
    //LOGCATE("MyGLRenderContext::OnDrawFrame");
    //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    if (m_pBeforeSample)
    {
        m_pBeforeSample->Destroy();
        delete m_pBeforeSample;
        m_pBeforeSample = nullptr;
    }
    if (m_pCurSample)
    {
        //m_pCurSample->Init();
        m_pCurSample->Draw(m_ScreenW, m_ScreenH);
    }
}

GLRenderContext *GLRenderContext::GetInstance()
{
    //LOGCATE("MyGLRenderContext::GetInstance");
    if (m_pContext == nullptr)
    {
        m_pContext = new GLRenderContext();
    }
    return m_pContext;
}

void GLRenderContext::DestroyInstance()
{
    //LOGCATE("MyGLRenderContext::DestroyInstance");
    if (m_pContext)
    {
        delete m_pContext;
        m_pContext = nullptr;
    }
}

