//
// Created by jianw on 2017/12/19 0019.
//

#ifndef INC_01_ASSIMP_MESH_H
#define INC_01_ASSIMP_MESH_H

#include <vector>
#include <string>
#include <iostream>
#include <shader.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

using std::cout;
using std::endl;
using std::string;
using std::vector;
/**
 * 顶点结构体
 */
struct Vertex {
    glm::vec3 Position;     // 位置向量
    glm::vec3 Normal;       // 法向量
    glm::vec2 TexCoords;    // 纹理坐标向量
};
/**
 * 纹理结构体
 */
struct Texture {
    unsigned int id;        // 纹理ID
    string type;            // 纹理类型
};
/**
 * 网格类
 */
class Mesh {
public:
    /*  网格数据  */
    vector<Vertex>          vertices;
    vector<Texture>         textures;
    vector<unsigned int>    indices;
    /*  函数  */
    Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices, vector<Texture>& textures);
    void Draw(Shader& shader);

private:
    /*  渲染数据  */
    unsigned int VAO, VBO, EBO;
    /*  初始化函数  */
    void setupMesh();
};


#endif //INC_01_ASSIMP_MESH_H
