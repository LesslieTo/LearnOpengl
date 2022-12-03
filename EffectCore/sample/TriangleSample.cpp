#include "TriangleSample.h"
#include "util/GLUtils.h"
TriangleSample::TriangleSample()
{
}

TriangleSample::~TriangleSample()
{
}

void TriangleSample::LoadImage(NativeImage *pImage)
{
    //null implement
}

void TriangleSample::Init()
{
    if(!gladLoadGL()){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    if(m_ProgramObj != 0)
        return;

    char vShaderStr[] =
    "#version 330 core                        \n"
    "layout(location = 0) in vec4 vPosition;  \n"
    "void main()                              \n"
    "{                                        \n"
    "   gl_Position = vPosition;              \n"
    "}                                        \n";


    char fShaderStr[] =
    "#version 330 core                            \n"
    "out vec4 fragColor;                          \n"
    "void main()                                  \n"
    "{                                            \n"
    "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
    "}                                            \n";

    m_ProgramObj =  GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);
}

void TriangleSample::Draw(int screenW, int screenH)
{
    //LOGCATE("TriangleSample::Draw");
    GLfloat vVertices[] = {
            0.0f,  0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if(m_ProgramObj == 0)
        return;

    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Use the program object
    glUseProgram (m_ProgramObj);

    // Load the vertex data
    //glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
    //glEnableVertexAttribArray (0);

    glBindVertexArray(VAO);
    glDrawArrays (GL_TRIANGLES, 0, 3);
    glUseProgram (GL_NONE);
}

void TriangleSample::Destroy()
{
    if (m_ProgramObj)
    {
        glDeleteProgram(m_ProgramObj);
        m_ProgramObj = GL_NONE;
    }
}
