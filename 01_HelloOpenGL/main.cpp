/***
 * 可以把OpenGL看成一个大型的状态机。
 */
#include <iostream>
// 静态链接GLEW 不需要把lib放到exe目录下了
#define GLEW_STATIC
// 先包含GLEW再包含GLFW 有依赖关系
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using namespace std;

/***
 * 按键事件回调函数
 *  回调函数需要使用 glfwSet...Callback(window, callback) 函数注册 注意：要在游戏循环前注册
 * @param window 响应事件的窗口
 * @param key 被按下的键
 * @param scanCode 暂时未知
 * @param action 动作(按下、释放..)
 * @param mode 是否有Ctrl等控制键
 */
void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mode);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 使用核心模式 这样在使用旧版本函数时会报错(invalid operation)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "HelloOpenGL中文", nullptr, nullptr);
    if (nullptr == window) {
        cout << "创建窗口失败" << endl;
        // 调用终止函数 释放资源
        glfwTerminate();
        return -1;
    }
    // 将窗口设置为当前线程的上下文
    glfwMakeContextCurrent(window);

    // 初始化GLEW GLEW用来管理OpenGL的函数指针 调用OpenGL函数前需要先初始化GLEW
    glewExperimental = GL_TRUE; // 可以使用更多现代化的技术
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    int width, height;
    // 从GLFW获取视口(Viewport)宽高
    glfwGetFramebufferSize(window, &width, &height);
    // 渲染窗口 前两个参数是左上角位置
    glViewport(0, 0, width, height);

    glfwSetKeyCallback(window, key_callback);
    // 游戏循环
    while(!glfwWindowShouldClose(window))
    {
        // 检查是否触发了事件
        glfwPollEvents();
        // 渲染指令
        /***
         * 把渲染(Rendering)操作放在游戏循环中
         */
        // 指定清空颜色后填充的颜色 RGBA
        // 状态设置函数
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        /***
         * 清空指定标识的缓冲
         *  GL_COLOR_BUFFER_BIT
         *  GL_DEPTH_BUFFER_BIT
         *  GL_STENCIL_BUFFER_BIT
         */
        // 状态应用函数
        glClear(GL_COLOR_BUFFER_BIT);
        // 交换颜色缓冲
        /***
         * 双缓冲(Double Buffer)
         *  应用程序使用单缓冲绘图时可能会存在图像闪烁的问题。
         *  这是因为生成的图像不是一下子被绘制出来的，而是按照从左到右，由上而下逐像素地绘制而成的。
         *  最终图像不是在瞬间显示给用户，而是通过一步一步生成的，这会导致渲染的结果很不真实。
         *  为了规避这些问题，我们应用双缓冲渲染窗口应用程序。
         *  前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。当
         *  所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲，这样图像就立即呈显出来，之前提到的不真实感就消除了。
         */
        glfwSwapBuffers(window);
    }

    // 释放资源
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mode) {
    // 按下ESC键时响应
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        // 设置关闭窗口属性为true
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}