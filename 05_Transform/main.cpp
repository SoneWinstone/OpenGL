/**
 * 缩放：
 *   [S1,  0,  0, 0
 *     0, S2,  0, 0
 *     0,  0, S3, 0
 *     0,  0,  0, 1 ]
 *  位移：
 *   [ 1,  0,  0, Tx
 *     0,  1,  0, Ty
 *     0,  0,  1, Tz
 *     0,  0,  0, 1 ]
 *
 * 矩阵组合：
 *  矩阵变换的顺序跟编程的顺序是相反的，最先定义的最后起作用
 *
 * GLM：
 *  专门为OpenGL量身定做的数学库。只要一个头文件就可以使用
 */
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "shader.h"
using namespace std;
const static string imgFolder = R"(C:\Users\jianw\Pictures\OpenGL\)";
const static int WIDTH = 800, HEIGHT = 600;
static float fMixVal = 0.2f;

// 按键处理函数
void processInput(GLFWwindow *window);
// 窗口大小改变的回调函数
void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    /**
     * 把一个向量(1,0,0)位移(1,0,0)个单位
     */
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    // 注意这里要初始化！！！为单位矩阵 不然结果是未定义的
    glm::mat4 tran(1.0);
    // translate：位移函数
    tran = glm::translate(tran, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = tran * vec; // Clion 会报错 实际上是没问题的
    cout << vec.x << " " << vec.y << " " << vec.z << endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "04_变换", nullptr, nullptr);
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
            .5f, -.5f, .0f,    0.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
            -.5f,  .5f, .0f,    1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
            .5f,  .5f, .0f,    1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

    shader.use();
    glUniform1i(glGetUniformLocation(shader.program, "texture1"), 0);
    shader.setInt("texture2", 1);
    shader.setFloat("fMix", fMixVal);

    glm::mat4 trans(1.0);
    /***
     * glm::radians：转成弧度制
     * glm::vec3(.0f, .0f, 1.0f)：x:0 y:0 z:1 -> 沿Z轴旋转
     */
    int location = glGetUniformLocation(shader.program, "transform");
//    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(.0f, .0f, 1.0f));
//    trans = glm::scale(trans, glm::vec3(.5f, .5f, .5f));
//    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(trans));
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 trans(1.0); // 每次重新定义
        // 实际顺序是先旋转后位移
        trans = glm::translate(trans, glm::vec3(.5f, -.5f, 0.0f));
        trans = glm::rotate(trans, static_cast<float>(glfwGetTime()), glm::vec3(.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(trans));

        // 默认GL_TEXTURE0 总是激活的 可以不写
        // glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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
