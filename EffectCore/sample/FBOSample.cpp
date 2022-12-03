#include "FBOSample.h"
#include "util/GLUtils.h"

#define VERTEX_POS_INDX  0
#define TEXTURE_POS_INDX 1
FBOSample::FBOSample()
{
}

FBOSample::~FBOSample()
{
    NativeImageUtil::FreeNativeImage(&m_RenderImage);
}

void FBOSample::LoadImage(NativeImage *pImage)
{
    //LOGCATE("FBOSample::LoadImage pImage = %p", pImage->ppPlane[0]);
    if (pImage)
    {
        m_RenderImage.width = pImage->width;
        m_RenderImage.height = pImage->height;
        m_RenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &m_RenderImage);
    }
}

void FBOSample::Init()
{
    if(!gladLoadGL()){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    //顶点坐标
    GLfloat vVertices[] = {
        -1.0f, -1.0f, 0.0f,  // bottom left
        1.0f, -1.0f, 0.0f,   // bottom right
        -1.0f,  1.0f, 0.0f,  // top left
        1.0f,  1.0f, 0.0f,   // top right
    };

    //正常纹理坐标
    GLfloat vTexCoors[] = {
        0.0f, 1.0f,        // bottom left
        1.0f, 1.0f,        // bottom right
        0.0f, 0.0f,        // top left
        1.0f, 0.0f,        // top right
    };

    //fbo 纹理坐标与正常纹理方向不同，原点位于左下角
    GLfloat vFboTexCoors[] = {
        0.0f, 0.0f,  // bottom left
        1.0f, 0.0f,  // bottom right
        0.0f, 1.0f,  // top left
        1.0f, 1.0f,  // top right
    };

    GLushort indices[] = { 0, 1, 2, 1, 3, 2 };

    char vShaderStr[] =
        "#version 330 core                          \n"
        "layout(location = 0) in vec4 a_position;   \n"
        "layout(location = 1) in vec2 a_texCoord;   \n"
        "out vec2 v_texCoord;                       \n"
        "void main()                                \n"
        "{                                          \n"
        "   gl_Position = a_position;               \n"
        "   v_texCoord = a_texCoord;                \n"
        "}                                          \n";

    // 用于普通渲染的片段着色器脚本，简单纹理映射
    char fShaderStr[] =
        "#version 330 core                          \n"
        "in vec2 v_texCoord;                        \n"
        "layout(location = 0) out vec4 outColor;    \n"
        "uniform sampler2D s_TextureMap;            \n"
        "void main()                                \n"
        "{                                          \n"
        "    outColor = texture(s_TextureMap, v_texCoord);\n"
        "}";

    // 用于离屏渲染的片段着色器脚本，取每个像素的灰度值
    char fFboShaderStr[] =
        "#version 330 core\n"
        "precision mediump float;\n"
        "in vec2 v_texCoord;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "uniform sampler2D s_TextureMap;\n"
        "void main()\n"
        "{\n"
        "    vec4 tempColor = texture(s_TextureMap, v_texCoord);\n"
        "    float luminance = tempColor.r * 0.299 + tempColor.g * 0.587 + tempColor.b * 0.114;\n"
        "    outColor = vec4(vec3(luminance), tempColor.a);\n"
        "}"; // 输出灰度图

        // 编译链接用于普通渲染的着色器程序
        m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);

        // 编译链接用于离屏渲染的着色器程序
        m_FboProgramObj = GLUtils::CreateProgram(vShaderStr, fFboShaderStr, m_FboVertexShader, m_FboFragmentShader);

        if (m_ProgramObj == GL_NONE || m_FboProgramObj == GL_NONE){
            std::cout<< "FBOSample::Init m_ProgramObj == GL_NONE" << std::endl;
            return;
        }

        m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
        m_FboSamplerLoc = glGetUniformLocation(m_FboProgramObj, "s_TextureMap");


        // 生成 VBO ，加载顶点数据和索引数据
        // Generate VBO Ids and load the VBOs with data
        glGenBuffers(4, m_VboIds);
        glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vTexCoors), vTexCoors, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vFboTexCoors), vFboTexCoors, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[3]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


        // 生成2个VAO, 一个用于普通渲染，另一个用于离屏渲染
        // Generate VAO Ids
        glGenVertexArrays(2, m_VaoIds);

        // 初始化用于普通渲染的 VAO
        // Normal rendering VAO
        glBindVertexArray(m_VaoIds[0]);

        glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
        glEnableVertexAttribArray(VERTEX_POS_INDX);
        glVertexAttribPointer(VERTEX_POS_INDX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void *)0);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

        glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
        glEnableVertexAttribArray(TEXTURE_POS_INDX);
        glVertexAttribPointer(TEXTURE_POS_INDX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const void *)0);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[3]);

        glBindVertexArray(GL_NONE);

#if 1
        // 初始化用于离屏渲染的 VAO
        // FBO off screen rendering VAO
        glBindVertexArray(m_VaoIds[1]);

        glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
        glEnableVertexAttribArray(VERTEX_POS_INDX);
        glVertexAttribPointer(VERTEX_POS_INDX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void *)0);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

        glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[2]);
        glEnableVertexAttribArray(TEXTURE_POS_INDX);
        glVertexAttribPointer(TEXTURE_POS_INDX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const void *)0);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[3]);

        glBindVertexArray(GL_NONE);


        // 创建并初始化图像纹理
        glGenTextures(1, &m_ImageTextureId);
        glBindTexture(GL_TEXTURE_2D, m_ImageTextureId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_RenderImage.width, m_RenderImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);

        if (!CreateFrameBufferObj_())
        {
            std::cout << ("FBOSample::Init CreateFrameBufferObj fail") << std::endl;
            return;
        }

 #endif

}

void FBOSample::Draw(int screenW, int screenH)
{
    // 离屏渲染
#if 1
    //glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    // Do FBO off screen rendering
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);
    glViewport(0, 0, m_RenderImage.width, m_RenderImage.height);


    // 选定离屏渲染的 Program，绑定 VAO 和图像纹理，进行绘制（离屏渲染）
    // m_ImageTextureId 为另外一个用于纹理映射的图片纹理
    glUseProgram(m_FboProgramObj);
    glBindVertexArray(m_VaoIds[1]);

    // 绑定图像纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ImageTextureId);

    glUniform1i(m_FboSamplerLoc, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *)0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // 解绑 FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

#endif
    // 完成离屏渲染后，结果图数据便保存在我们之前连接到 FBO 的纹理 m_FboTextureId 。
    // 我们再拿 FBO 纹理 m_FboTextureId 做一次普通渲染便可将之前离屏渲染的结果绘制到屏幕上。
    // 这里我们编译连接了 2 个 program ,一个用作离屏渲染的 m_FboProgramObj，一个用于普通渲染的 m_ProgramObj
    //选定另外一个着色器程序，以 m_FboTextureId 纹理作为输入进行普通渲染

#if 1
    // 普通渲染
    // Do normal rendering
    glViewport(0, 0, screenW, screenH);
    glUseProgram(m_ProgramObj);
    glBindVertexArray(m_VaoIds[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_FboTextureId);
    glUniform1i(m_SamplerLoc, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *)0);

    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(GL_NONE);

#endif
}

void FBOSample::Destroy()
{
    if (m_ProgramObj){
        glDeleteProgram(m_ProgramObj);
    }

    if (m_FboProgramObj){
        glDeleteProgram(m_FboProgramObj);
    }

    if (m_ImageTextureId){
        glDeleteTextures(1, &m_ImageTextureId);
    }

    if (m_FboTextureId){
        glDeleteTextures(1, &m_FboTextureId);
    }

    if (m_VboIds[0]){
        glDeleteBuffers(4, m_VboIds);
    }

    if (m_VaoIds[0]){
        glDeleteVertexArrays(2, m_VaoIds);
    }

    if (m_FboId){
        glDeleteFramebuffers(1, &m_FboId);
    }
}

bool FBOSample::CreateFrameBufferObj_()
{
//    // 创建并初始化 FBO 纹理
    glGenTextures(1, &m_FboTextureId);
    glBindTexture(GL_TEXTURE_2D, m_FboTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    // 创建并初始化 FBO
    glGenFramebuffers(1, &m_FboId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);
    glBindTexture(GL_TEXTURE_2D, m_FboTextureId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FboTextureId, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_RenderImage.width, m_RenderImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!= GL_FRAMEBUFFER_COMPLETE) {
        std::cout << ("FBOSample::CreateFrameBufferObj glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE")<< std::endl;
        return false;
    }
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    return true;
}


bool FBOSample::CreateFrameBufferObj()
{
    // 创建并初始化 FBO 纹理
    glGenTextures(1, &m_FboTextureId);
    glBindTexture(GL_TEXTURE_2D, m_FboTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //NULL means reserve texture memory, but texels are undefined
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_RenderImage.width, m_RenderImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);


    // 创建深度缓冲区
    glGenRenderbuffers(1, &m_depthId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_RenderImage.width, m_RenderImage.height);


    // 创建并初始化 FBO
    glGenFramebuffers(1, &m_FboId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);


    //Attach 2D texture to this FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
                           GL_COLOR_ATTACHMENT0,  // 2. attachment point
                           GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                           m_FboTextureId,        // 4. tex ID
                           0);

    //Attach depth buffer to FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,              // 1. fbo target: GL_FRAMEBUFFER
                              GL_DEPTH_STENCIL_ATTACHMENT, // 2. attachment point
                              GL_RENDERBUFFER,             // 3. rbo target: GL_RENDERBUFFER
                              m_depthId);                  // 4. rbo ID


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!= GL_FRAMEBUFFER_COMPLETE) {
        std::cout<< ("FBOSample::CreateFrameBufferObj glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE") << std::endl;
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    return true;
}

