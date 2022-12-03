#include "TextureMapSample.h"
#include "util/GLUtils.h"
#include "external/glad/glad.h"
TextureMapSample::TextureMapSample()
{
    m_TextureId = 0;
}

TextureMapSample::~TextureMapSample()
{
    NativeImageUtil::FreeNativeImage(&m_RenderImage);
}

void TextureMapSample::Init()
{
    if(!gladLoadGL()){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

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

    char fShaderStr[] =
            "#version 330 core                                   \n"
            "in vec2 v_texCoord;                                 \n"
            "out vec4 outColor;                                  \n"
            "uniform sampler2D s_TextureMap;                     \n"
            "void main()                                         \n"
            "{                                                   \n"
            "  outColor = texture(s_TextureMap, v_texCoord);     \n"
            "}                                                   \n";


    const GLfloat VERTEX_DATA[] = {
        0.5f,  0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f,
        -0.5f,  0.5f
    };
    const GLuint INDEX_DATA[] = {
        0, 1, 3,
        1, 2, 3
    };

    const GLfloat TEXTURE_COORDS[] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };


    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);
    if (m_ProgramObj)
    {
        m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
    }
    else
    {
        std::cout<< ("TextureMapSample::Init create program fail") << std::endl;
    }


    // Generate VBO Ids and load the VBOs with data
    glGenBuffers(3, m_VboIds);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_DATA), VERTEX_DATA, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TEXTURE_COORDS), TEXTURE_COORDS, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDEX_DATA), INDEX_DATA, GL_STATIC_DRAW);


    // Generate VAO Ids
    glGenVertexArrays(1, m_VaoIds);

    // FBO off screen rendering VAO
    glBindVertexArray(m_VaoIds[0]);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[2]);
    glBindVertexArray(GL_NONE);
}

void TextureMapSample::LoadImage(NativeImage* pImage)
{
    if (pImage)
    {
        m_RenderImage.width = pImage->width;
        m_RenderImage.height = pImage->height;
        m_RenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &m_RenderImage);
    }

    //bind texture
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_RenderImage.width, m_RenderImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}


void TextureMapSample::Draw(int sceenW, int sceenH)
{
    if(m_ProgramObj == GL_NONE || m_TextureId == GL_NONE) return;
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


#if 1
    glUseProgram(m_ProgramObj);
    glBindVertexArray(m_VaoIds[0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glUniform1i(m_SamplerLoc, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  //画矩形背景

    glBindVertexArray(GL_NONE);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

#endif

#if 0
    //upload RGBA image data
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_RenderImage.width, m_RenderImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    glUseProgram(m_ProgramObj);

    // Load the vertex position
    glVertexAttribPointer (0, 3, GL_FLOAT,
                            GL_FALSE, 3 * sizeof (GLfloat), verticesCoords);
    // Load the texture coordinate
    glVertexAttribPointer (1, 2, GL_FLOAT,
                            GL_FALSE, 2 * sizeof (GLfloat), textureCoords);

    glEnableVertexAttribArray (0);
    glEnableVertexAttribArray (1);

    // Bind the RGBA map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);

    // Set the RGBA map sampler to texture unit to 0
    glUniform1i(m_SamplerLoc, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
#endif

}

void TextureMapSample::Destroy()
{
    if (m_ProgramObj)
    {
        glDeleteProgram(m_ProgramObj);
        glDeleteTextures(1, &m_TextureId);
        m_ProgramObj = GL_NONE;
    }
}
