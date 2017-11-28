/**
 * 使用uniform定义一个水平偏移量，在顶点着色器中使用这个偏移量把三角形移动到屏幕右侧
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
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "03test02_水平偏移", nullptr, nullptr);
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
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat))); // 步长：7 偏移：3F
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    Shader shader = Shader("../vertex.shader", "../fragment.shader");
    GLfloat green, offset = 0.0f;
    // 查找"greenValue"的位置 没找到返回-1
    GLint greenLocation = glGetUniformLocation(shader.program, "greenValue");
    GLint offsetLocation = glGetUniformLocation(shader.program, "HorizontalOffset");

    glViewport(0, 0, WIDTH, HEIGHT);

    glfwSetKeyCallback(window, key_callback);
    bool ret = false;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        auto time = glfwGetTime();
        green = static_cast<GLfloat>((sin(time) / 2) + 0.5);

        if (offset > 0.49F)
            ret = true;
        if (offset < -0.49F)
            ret = false;
        if (ret)
            offset -= 0.01;
        else
            offset += 0.01;

        glUniform1f(greenLocation, green);
        glUniform1f(offsetLocation, offset);
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