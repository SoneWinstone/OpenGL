/***
 * 尝试用不同的纹理环绕方式，设定一个从0.0f到2.0f范围内的（而不是原来的0.0f到1.0f）纹理坐标。试试看能不能在箱子的角落放置4个笑脸
 * 环绕方式：
 *      GL_REPEAT	对纹理的默认行为。重复纹理图像。
 *      GL_MIRRORED_REPEAT	和GL_REPEAT一样，但每次重复图片是镜像放置的。
 *      GL_CLAMP_TO_EDGE	纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
 *      GL_CLAMP_TO_BORDER	超出的坐标为用户指定的边缘颜色。
 */
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "shader.h"
using namespace std;
const string imgFolder = R"(C:\Users\jianw\Pictures\OpenGL\)";
const int WIDTH = 800, HEIGHT = 600;

// 按键处理函数(非回调版)
void processInput(GLFWwindow *window);
// 窗口大小改变的回调函数
void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "04_纹理", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

    Shader shader = Shader("../vertex.shader", "../fragment.shader");

    GLfloat vertices[] = {
            // 位置              // 颜色                    // 纹理坐标
            -.5f, -.5f, .0f,    1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 0.0f,
             .5f, -.5f, .0f,    0.0f, 1.0f, 1.0f, 1.0f,   2.0f, 0.0f,
            -.5f,  .5f, .0f,    1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 2.0f,
             .5f,  .5f, .0f,    1.0f, 1.0f, 0.0f, 1.0f,   2.0f, 2.0f
    };
    GLuint indexes[] = {
            0, 1, 2,
            1, 2, 3
    };

    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *) nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *) (7 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    GLuint texture[2];
    glGenTextures(2, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nChannels;
    unsigned char* image = stbi_load((imgFolder + "container.jpg").c_str(), &width, &height, &nChannels, 0);
    if (image) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load texture!" << endl;
    }
    stbi_image_free(image);

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 设置加载图像时翻转Y轴
    stbi_set_flip_vertically_on_load(true);
    image = stbi_load((imgFolder + "awesomeface.png").c_str(), &width, &height, &nChannels, 0);
    if (image) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load texture!" << endl;
    }
    stbi_image_free(image);

    // 要在设置uniform 之前激活shader！！！
    shader.use();
    // 设置采样器属于哪个纹理单元(纹理的位置)
    glUniform1i(glGetUniformLocation(shader.program, "texture1"), 0);
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

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}