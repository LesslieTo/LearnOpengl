#ifndef EFFECTCOREVIEW_H
#define EFFECTCOREVIEW_H
#include "render/GLRenderContext.h"
#include <iostream>
class EffectCoreView
{
public:
    EffectCoreView();
    ~EffectCoreView();

    void initializeGL();			/*写渲染前的配置信息*/
    void resizeGL(int w, int h);	/*窗口变化*/
    void paintGL();                 /*写渲染循环的内容*/


    //void setImageData(const std::string &path);

private:
    int m_SceenW {0};
    int m_SceenH {0};
    //GLSampleBase* m_pCurSample;
};

#endif // EFFECTCOREVIEW_H
