//
// Created by jianw on 2017/12/19 0019.
//

#include "Mesh.h"

/**
 * 在真正渲染这个网格之前，我们需要在调用glDrawElements函数之前先绑定相应的纹理。
 * 然而，这实际上有些困难，我们一开始并不知道这个网格（如果有的话）有多少纹理、纹理是什么类型的。
 * 为了解决这个问题，我们需要设定一个命名标准：
 *   每个漫反射纹理被命名为texture_diffuseN，每个镜面光纹理应该被命名为texture_specularN，其中N的范围是1到纹理采样器最大允许的数字。
 */
void Mesh::Draw(Shader& shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
        // 获取纹理序号（diffuse_textureN 中的 N）
        string number;
        string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.setFloat("material." + name + number, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // 绘制网格
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

Mesh::Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices, vector<Texture>& textures)
    : vertices(vertices),
      textures(textures),
      indices(indices) {
    setupMesh();
}

void Mesh::setupMesh() {
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    // 顶点法线
    /**
     * 结构体的数据在内存中是连续的
     * offsetof(struct, variableName) : 返回变量距结构体头部的字节偏移
     */
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(offsetof(Vertex, Normal)));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(offsetof(Vertex, TexCoords)));

    glBindVertexArray(0);
}
