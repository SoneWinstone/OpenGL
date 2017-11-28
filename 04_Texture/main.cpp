/**
 * 使用out关键字把顶点位置输出到片段着色器，并将片段的颜色设置为与顶点位置相等（来看看连顶点位置值都在三角形中被插值的结果）。
 * 做完这些后，尝试回答下面的问题：为什么在三角形的左下角是黑的?
 * 回答：左下角的坐标 <= 0.0F 转换成颜色值为0.0F 所以是黑色
 */
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <string>
#include <iostream>
//#include <SOIL/SOIL.h>
#include "shader.h"
using namespace std;
const string imgFolder = R"(C:\Users\jianw\Pictures\OpenGL\)";
const int WIDTH = 800, HEIGHT = 600;

// 按键处理函数(非回调版)
void processInput(GLFWwindow *window);
// 窗口大小改变的回调函数
void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mode);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "04_纹理", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
//    glfwSetKeyCallback(window, key_callback);

    Shader shader = Shader("../vertex.shader", "../fragment.shader");

    GLfloat vertices[] = {
            // 位置              // 颜色                    // 纹理坐标
            -.5f, -.5f, .0f,    1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 0.0f,
             .5f, -.5f, .0f,    0.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
            -.5f,  .5f, .0f,    1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
             .5f,  .5f, .0f,    1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f
    };
    GLuint indexes[] = {
            0, 1, 2,
            1, 2, 3
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *) nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4/*长度*/, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat))); // 步长：9 偏移：3F
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *) (7 * sizeof(GLfloat))); // 步长：9 偏移：7F
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    GLuint texture[2];
    glGenTextures(2, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    // 对当前绑定的纹理对象设置环绕、过滤方式
    /***
     * param1：纹理目标 1D/2D/3D..
     * param2：设置的选项和纹理轴 这里选项是Warp变形 方向S和T(对应xy，R对应z)
     * param3：纹理环绕方式
     *
     * 如果我们选择GL_CLAMP_TO_BORDER选项，我们还需要指定一个边缘的颜色。
     * 这需要使用glTexParameter函数的fv后缀形式，用GL_TEXTURE_BORDER_COLOR作为它的选项，并且传递一个float数组作为边缘的颜色值：
     *  float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
     *  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /**
     * param2：选项：放大Magnify 缩小Minify
     * param3：过滤方式 NEAREST邻近：锯齿 清晰 LINEAR线性：平滑 模糊
     */
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /***
     * 多级渐远纹理Mipmap 距离观察者越远 解析度越低
     * 四种过滤方式：
     *  GL_NEAREST_MIPMAP_NEAREST	使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样
     *  GL_LINEAR_MIPMAP_NEAREST	使用最邻近的多级渐远纹理级别，并使用线性插值进行采样
     *  GL_NEAREST_MIPMAP_LINEAR	在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样
     *  GL_LINEAR_MIPMAP_LINEAR	    在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样
     *
     * 注意：不能对放大使用多级渐远纹理过滤选项，不然会报错GL_INVALID_ENUM
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    // 使用SOIL加载图像
//    unsigned char* image = SOIL_load_image((imgFolder + "wall.jpg").c_str(), &width, &height, 0/*通道数*/, SOIL_LOAD_RGB);
    // 使用stb_image.h 加载图像
    int width, height, nChannels;
    unsigned char* image = stbi_load((imgFolder + "container.jpg").c_str(), &width, &height, &nChannels, 0);
    /***
     * param1：纹理目标 与当前绑定的纹理保持一致
     * param2：多级渐远纹理级别
     * param3：希望存储的格式
     * ...
     * param7：源图格式
     * param8：源图数据类型
     * param9：源图
     */
    if (image) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0/*固定值0 历史遗留问题*/, GL_RGB, GL_UNSIGNED_BYTE, image);
        // 为当前绑定的纹理对象自动生成需要的多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load texture!" << endl;
    }
    // 释放SOIL的资源
//    SOIL_free_image_data(image);
    stbi_image_free(image);

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /**
     * OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。所以默认加载的图片上下是颠倒的。
     */
    // 设置加载图像时翻转Y轴
    stbi_set_flip_vertically_on_load(true);
    image = stbi_load((imgFolder + "awesomeface.png").c_str(), &width, &height, &nChannels, 0);
    // 注意这里的第七个参数是 GL_RGBA！ GL_RGBA！ GL_RGBA！
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);
    // glBindTexture(GL_TEXTURE_2D, 0);

    // 要在设置uniform 之前激活shader！！！
    shader.use();
    // 设置采样器属于哪个纹理单元(纹理的位置)
    glUniform1i(glGetUniformLocation(shader.program, "texture1"), 0);
//    glUniform1i(glGetUniformLocation(shader.program, "texture2"), 1);
    shader.setInt("texture2", 1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 默认GL_TEXTURE0 总是激活的
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mode) {
    cout << "ScanCode: " << scanCode << endl;
    cout << "Mode: " << mode << endl;

    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
    // 改变窗口大小时重新设置视口
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}