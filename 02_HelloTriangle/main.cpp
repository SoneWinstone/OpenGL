/***
 * OpenGL中，任何事物都在3D空间命中。由3D坐标转为2D坐标的处理过程是由图形渲染管线(Graphics Pipeline，通常叫管线)管理的。
 * 图形渲染管线可以划分为几个固定的阶段，每个阶段会把前一个阶段的输出作为输入，这些阶段很容易并行执行。GPU成千上万的小处理核心可以为每个阶段运行各自的小程序(着色器 Shader)
 * 顶点着色器(Vertex Shader) -> 图元装配(Shape Assembly) -> 几何着色器(Geometry Shader) -> 光栅化(Rasterization) -> 片段着色器(Fragment Shader) -> 测试与混合(Tests and Blending)
 * 在现代OpenGL中，必须定义至少一个顶点着色器和一个片段着色器(GPU中没有默认的顶点/片段着色器)
 */
#include <iostream>
#include <glad/glad.h>
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

    // 初始化GLAD GLAD用来管理OpenGL的函数指针 调用OpenGL函数前需要先初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "初始化 GLAD 失败" << endl;
        return -1;
    }

    // 定义三角形顶点 标准化设备坐标(Normalized Device Coordinates, NDC)  超过-1.0或1.0的数据点不会处理 显示范围[-1.0, 1.0]
    GLfloat vertices[] = {
            -0.5f, -0.5f, .0f,
            .5f, -0.5f, .0f,
            .0f, .5f, .0f
    };
    // 定义矩形点(4个)
    GLfloat rect[] = {
            0.5f, 0.5f, 0.0f,   // 右上角
            0.5f, -0.5f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f, // 左下角
            -0.5f, 0.5f, 0.0f   // 左上角
    };
    // 定义索引
    GLuint indices[] = { // 注意索引从0开始!
            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
    };

//------------------------------------------------   VBO    ------------------------------------------------------------
    // 顶点缓冲对象(Vertex Buffer Objects, VBO) 用于保存顶点信息 CPU发送数据到GPU相对较慢 使用缓冲可以一次发送大量数据
    GLuint VBO, VBO_RECT;
    /**
     * 生成Buffer
     * param1：需要生成的buffer数量
     * param2：BufferID 可以是数组(param1 > 1)
     */
    glGenBuffers(1, &VBO);
    // OpenGL中有多个缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER。可以同时绑定到多个不同类型的缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 把定义的顶点数据复制到“当前绑定”的缓冲的内存中
    /**
     * 把用户定义的数据复制到当前绑定的缓冲中
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

    glGenBuffers(1, &VBO_RECT);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_RECT);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------  编写着色器  -----------------------------------------------------------
    /// 使用着色器语言(OpenGL Shading Language, GLSL)编写顶点着色器
    /***
     * 着色器源码
     * version 330 core -> 对应OpenGL的版本
     * (location = 0) 输入位置
     * in 输入
     * vec3 3维数据
     * gl_Position 顶点着色器的输出，vec4类型
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
    /**
     * glCreateShader param: 着色器类型
     */
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 把着色器源码附加到着色器对象上 然后编译
    /**
     * glShaderSource 设置着色器源码
     *  param1：着色器对象
     *  param2：源码数量
     *  param3：源码
     */
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // 检查是否编译成功
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /**
     * 定义片段着色器源码 RGBA
     * 片段着色器只需要一个输出变量表示最终输出的颜色
     */
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
        std::cout << "编译着色器程序失败！" << std::endl;
        return -1;
    }
    // 把着色器对象链接到程序对象以后, 删除着色器对象, 不需要了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 激活程序对象
    // 在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象了。
    // glUseProgram(shaderProgram);
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
    // VAO会记录：
    //      1. glEnableVertexAttribArray和glDisableVertexAttribArray的调用
    //      2. 通过glVertexAttribPointer设置的顶点属性配置
    //      3.通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象

    // ..:: 初始化代码（只运行一次 (除非你的物体频繁改变)） :: ..
    // 1. 绑定VAO
    glBindVertexArray(VAO);
    // 2. 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. 设置顶点属性指针
    // 每个顶点属性从一个VBO管理的内存中获得它的数据，而具体是从哪个VBO（程序中可以有多个VBO）获取则是通过在调用glVertexAttribPointer时绑定到GL_ARRAY_BUFFER的VBO决定的。
    // 由于在调用glVertexAttribPointer之前绑定的是先前定义的VBO对象，顶点属性0现在会链接到它的顶点数据。
    // 以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
    /**
     * 告诉OpenGL如何解释这些顶点数据
     * glVertexAttribPointer 链接顶点属性 从最后绑定到GL_ARRAY_BUFFER的VBO中获取顶点数据
     *  param1：指定需要配置的顶点属性 这里的0 与GLSL定义的顶点着色器 layout(location = 0) 相对应 把数据传递到了着色器对应的顶点属性中
     *  param2：指定顶点属性的大小 vec3 由3个值组成 所以传3
     *  param3：指定数据的类型(GLSL中vec*都是由浮点数值组成的)
     *  param4：是否希望数据被标准化(映射到0(有符号型数据-1)到1之间)
     *  param5：Step 步长 两个顶点相差的字节大小
     *  param6：表示位置数据在缓冲中起始位置的偏移量
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    // 启用VAO中对应的顶点属性数组,默认是关闭的 param:顶点属性位置(location)
    glEnableVertexAttribArray(0);
//    glDisableVertexAttribArray(0);
    // 4. 解绑VAO
    // 通常情况下当我们配置好OpenGL对象以后要解绑它们，这样我们才不会在其它地方错误地配置它们。
    glBindVertexArray(0);
    /* ..:: 绘制代码（游戏循环中） :: ..
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // someOpenGLFunctionThatDrawsOurTriangle();
    glBindVertexArray(0);
    */

    GLuint VAO_RECT;
    glGenVertexArrays(1, &VAO_RECT);
    glBindVertexArray(VAO_RECT);
    glBindBuffer(GL_ARRAY_BUFFER, VAO_RECT);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
//----------------------------------------------------------------------------------------------------------------------

//------------------------------------------------       EBO/IBO       -------------------------------------------------
    // 索引缓冲对象(Element Buffer Object，EBO，也叫Index Buffer Object，IBO)
    // 举例：如果没有EBO，绘制一个矩形需要6个顶点(OpenGL主要处理三角形，1Rect=2Triangle)，其中有两个点是重复的(对角点)，使用EBO就不必重复定义点了
    // 创建EBO
    GLuint EBO;
    glGenBuffers(1, &EBO);

    // 绑定VAO
    glBindVertexArray(VAO_RECT);

    // 先绑定EBO然后用glBufferData把索引复制到缓冲里
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 当VAO处于绑定状态时，正在绑定的索引缓冲对象会被保存为VAO的元素缓冲对象。绑定VAO的同时也会自动绑定EBO。
    // 当目标是GL_ELEMENT_ARRAY_BUFFER的时候，VAO会储存glBindBuffer的函数调用。
    // 这也意味着它也会储存解绑调用，所以应该先解绑VAO再解绑EBO，否则它就没有这个EBO配置了。
    // 解绑VAO
    glBindVertexArray(0);

//----------------------------------------------------------------------------------------------------------------------
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // 线框模式(Wireframe Mode)
        // 要想用线框模式绘制你的三角形，你可以通过glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)函数配置OpenGL如何绘制图元。
        // 第一个参数表示我们打算将其应用到所有的三角形的正面和背面，第二个参数告诉我们用线来绘制。
        // 之后的绘制调用会一直以线框模式绘制三角形，直到我们用glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)将其设置回默认模式。
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Draw triangle
        // 绑定VA0
        glBindVertexArray(VAO);
        // 画出想要的图形
        /**
         * param1：需要绘制的OpenGL的图元类型
         * param2：指定顶点数组的起始位置
         * param3：需要绘制的点数
         */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /**
         * 使用EBO绘制图形
         */
        /**
         * param1：绘制的图形
         * param2：顶点的个数
         * param3：索引的类型
         * param4：EBO中的偏移量
         */
        glBindVertexArray(VAO_RECT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, (GLvoid *) nullptr);
//        glDrawElements(GL_TRIANGLE_FAN, 3, GL_UNSIGNED_INT, (GLvoid *) (3 * sizeof(GLfloat)));
        // 解绑VA0
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VAO_RECT);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO_RECT);

    // 回收由GLFW申请的资源
    glfwTerminate();
    return 0;
}

/**
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
// 最终的初始化和绘制代码形式如下：
/**
// ..:: 初始化代码 :: ..
// 1. 绑定顶点数组对象
glBindVertexArray(VAO);
// 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
// 3. 设定顶点属性指针
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);
// 4. 解绑VAO（不是EBO！）
glBindVertexArray(0);

...

// ..:: 绘制代码（游戏循环中） :: ..
glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0)
glBindVertexArray(0);
*/