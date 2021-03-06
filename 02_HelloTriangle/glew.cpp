#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using namespace std;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "HelloTriangle", nullptr, nullptr);
    if (nullptr == window) {
        cout << "创建窗口失败" << endl;
        // 调用终止函数 释放资源
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // 定义顶点 标准化设备坐标(Normalized Device Coordinates, NDC)  超过-1.0或1.0的数据点不会处理 显示范围[-1.0, 1.0]
    GLfloat vertices[] = {
            -0.5f, -0.5f, .0f,
            .5f, -0.5f, .0f,
            .0f, .5f, .0f
    };

//------------------------------------------------   VBO    ------------------------------------------------------------
    // 顶点缓冲对象(Vertex Buffer Objects, VBO) 用于保存顶点信息 CPU发送数据到GPU相对较慢 使用缓冲可以一次发送大量数据
    GLuint VBO;
    glGenBuffers(1, &VBO);
    // OpenGL中有多个缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER。可以同时绑定多个不同类型的缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 把定义的顶点数据复制到“当前绑定”的缓冲的内存中
    /***
     * 专门用来把用户定义的数据复制到当前绑定缓冲的函数
     * glBufferData
     *  param1：目标缓冲的类型
     *  param2：传输数据的大小
     *  param3：需要发送的数据
     *  param4：显卡管理数据的方式：
     *      GL_STATIC_DRAW：数据几乎不会变
     *      GL_DYNAMIC_DRAW：数据会改变很多
     *      GL_STREAM_DRAW：数据每次绘制都会变
     */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /// 使用着色器语言(OpenGL Shading Language, GLSL)编写顶点着色器
    /***
     * 着色器源码
     * version 330 core -> 对应OpenGL的版本
     * (location = 0) 输入位置
     * in 输入
     * vec3 3维数据
     */
    const GLchar* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 position;

    void main() {
        gl_Position = vec4(position.x, position.y, position.z, 1.0);
    }
    )";
    // 创建顶点着色器对象
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 把着色器源码附加到着色器对象上 然后编译
    /***
     * param1：着色器对象
     * param2：源码数量
     * param3：源码
     */
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // 检查是否编译成功
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    const GLchar* fragmentShaderSource = R"(
    #version 330 core
    out vec4 color;
    void main() {
        color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
    )";
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // 着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本。
    // 如果要使用刚才编译的着色器我们必须把它们链接为一个着色器程序对象，然后在渲染对象的时候激活这个着色器程序。
    // 已激活着色器程序的着色器将在我们发送渲染调用的时候被使用。
    // 创建着色器程序
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    // 附加之前编译的着色器到程序对象上
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // 链接
    glLinkProgram(shaderProgram);
    // 检查是否链接成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        // ...
    }
    // 激活程序对象
    // 在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象了。
    glUseProgram(shaderProgram);
    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /***
     * glVertexAttribPointer
     *  param1：指定需要配置的顶点属性 这里的0 与GLSL定义的顶点着色器 layout(location = 0) 相对应 把数据传递到了着色器对应的顶点属性中
     *  param2：指定顶点属性的大小 vec3 由3个值组成 所以传3
     *  param3：指定数据的类型(GLSL中vec*都是由浮点数值组成的)
     *  param4：是否希望数据被标准化(映射到0(有符号型数据-1)到1之间)
     *  param5：Step 步长 两个顶点相差的字节大小
     *  param6：表示位置数据在缓冲中起始位置的偏移量
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    // 每个顶点属性从一个VBO管理的内存中获得它的数据，而具体是从哪个VBO（程序中可以有多个VBO）获取则是通过在调用glVetexAttribPointer时绑定到GL_ARRAY_BUFFER的VBO决定的。
    // 由于在调用glVetexAttribPointer之前绑定的是先前定义的VBO对象，顶点属性0现在会链接到它的顶点数据。
    // 以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
    glEnableVertexAttribArray(0);
//----------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------       VAO       ---------------------------------------------------
    // 顶点数组对象(Vertex Array Object, VAO)
    // 可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中。
    // 当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了。
    // 这使在不同顶点数据和属性配置之间切换变得非常简单，只需要绑定不同的VAO就行了。刚刚设置的所有状态都将存储在VAO中

    /*****   OpenGL的核心模式要求我们使用VAO，所以它知道该如何处理我们的顶点输入。如果我们绑定VAO失败，OpenGL会拒绝绘制任何东西。   *****/
    // 创建VAO对象
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // 要想使用VAO，要做的只是使用glBindVertexArray绑定VAO。
    // 从绑定之后起，我们应该绑定和配置对应的VBO和属性指针，之后解绑VAO供之后使用。
    // 打算绘制一个物体的时候，只要在绘制物体前简单地把VAO绑定到希望使用的设定上就行了
    // ..:: 初始化代码（只运行一次 (除非你的物体频繁改变)） :: ..
    // 1. 绑定VAO
    glBindVertexArray(VAO);
    // 2. 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 4. 解绑VAO
    // 通常情况下当我们配置好OpenGL对象以后要解绑它们，这样我们才不会在其它地方错误地配置它们。
    glBindVertexArray(0);
    // ..:: 绘制代码（游戏循环中） :: ..
    // 5. 绘制物体
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // someOpenGLFunctionThatDrawsOurTriangle();
    glBindVertexArray(0);
//----------------------------------------------------------------------------------------------------------------------

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

/***
 * 我们使用一个顶点缓冲对象将顶点数据初始化至缓冲中，建立了一个顶点和一个片段着色器，并告诉了OpenGL如何把顶点数据链接到顶点着色器的顶点属性上。
 *
 * 在OpenGL中绘制一个物体，代码会像是这样：
 *  // 0. 复制顶点数组到缓冲中供OpenGL使用
 *  glBindBuffer(GL_ARRAY_BUFFER, VBO);
 *  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 *  // 1. 设置顶点属性指针
 *  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
 *  glEnableVertexAttribArray(0);
 *  // 2. 当我们渲染一个物体时要使用着色器程序
 *  glUseProgram(shaderProgram);
 *  // 3. 绘制物体
 *  someOpenGLFunctionThatDrawsOurTriangle();
 */