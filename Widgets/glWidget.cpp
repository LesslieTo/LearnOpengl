#include "glWidget.h"

glWidget::glWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

glWidget::~glWidget()
{
}

void glWidget::initializeGL()
{
    m_CoreviewPtr->initializeGL();
    //m_CoreviewPtr->setImageData("/Users/yafei.du/Desktop/Console/container.jpeg");
}

void glWidget::resizeGL(int w, int h)
{
    m_CoreviewPtr->resizeGL(w,h);
}

void glWidget::paintGL()
{
    m_CoreviewPtr->paintGL();

//    GLuint fb = context()->defaultFramebufferObject();
//    glBindFramebuffer(GL_FRAMEBUFFER, fb);

}
