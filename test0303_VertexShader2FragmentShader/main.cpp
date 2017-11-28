/**
 * 使用out关键字把顶点位置输出到片段着色器，并将片段的颜色设置为与顶点位置相等（来看看连顶点位置值都在三角形中被插值的结果）。
 * 做完这些后，尝试回答下面的问题：为什么在三角形的左下角是黑的?
 * 回答：左下角的坐标 <= 0.0F 转换成颜色值为0.0F 所以是黑色
 */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include "shader.h"
using namespace std;
const int WIDTH = 800, HEIGHT = 600;
void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mode);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "03test03_着色器传值", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    GLfloat vertices[] = {
            // 位置              // 颜色
             .0f,  .5f, .0f,    1.0f, 0.0f, 0.0f, 1.0f,
            -.5f, -.5f, .0f,    0.0f, 1.0f, 0.0f, 1.0f,
             .5f, -.5f, .0f,    0.0f, 0.0f, 1.0f, 1.0f
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *) nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat))); // 步长：7 偏移：3F
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    Shader shader = Shader("../vertex.shader", "../fragment.shader");
    GLfloat offsetV, offsetH = 0.0f;
    // 查找"greenValue"的位置 没找到返回-1
    GLint offsetVLocation = glGetUniformLocation(shader.program, "VerticalOffset");
    GLint offsetHLocation = glGetUniformLocation(shader.program, "HorizontalOffset");

    glViewport(0, 0, WIDTH, HEIGHT);

    glfwSetKeyCallback(window, key_callback);
    bool ret = false;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        if (offsetH > 0.49F)
            ret = true;
        if (offsetH < -0.49F)
            ret = false;
        if (ret) {
            offsetH -= 0.01;
            offsetV -= 0.01;
        } else {
            offsetH += 0.01;
            offsetV += 0.01;
        }

        glUniform1f(offsetHLocation, offsetH);
        glUniform1f(offsetVLocation, offsetV);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

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