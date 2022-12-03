#ifndef TRIANGLESAMPLE_H
#define TRIANGLESAMPLE_H

/**
 * @brief 绘制三角形
*/

#include "GLSampleBase.h"

class TriangleSample : public GLSampleBase
{
public:
    TriangleSample();
    virtual ~TriangleSample();

    virtual void LoadImage(NativeImage *pImage);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();
};

#endif // TRIANGLESAMPLE_H
