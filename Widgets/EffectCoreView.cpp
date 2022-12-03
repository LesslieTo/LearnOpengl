
#include "EffectCoreView.h"

EffectCoreView::EffectCoreView()
{
    GLRenderContext::GetInstance();
}

EffectCoreView::~EffectCoreView()
{
    GLRenderContext::DestroyInstance();
}

void EffectCoreView::initializeGL()
{
    GLRenderContext::GetInstance()->SetParamsInt(SAMPLE_TYPE, SAMPLE_TYPE_KEY_TEXTURE_MAP, 0);
}

void EffectCoreView::resizeGL(int w, int h)
{
    //glGetIntegerv(GL_VIEWPORT, wh);
    //glViewport(Vector4(wh[0], wh[1], wh[2], wh[3]));
    //glViewport(0, 0, w, h);
    GLRenderContext::GetInstance()->OnSurfaceChanged(w, h);
}

void EffectCoreView::paintGL()
{
    GLRenderContext::GetInstance()->OnDrawFrame();
}

//void EffectCoreView::setImageData(const std::string &path)
//{
//    int width, height, channel;
//    auto data = stbi_load(path.c_str(), &width, &height, &channel, 0);
//    if(data){
//         std::cout << "Image read success !" << std::endl;
//        GLRenderContext::GetInstance()->SetImageData(IMAGE_FORMAT_P010, width, height, data);
//    }
//    stbi_image_free(data);
//}
