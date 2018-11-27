//
// Created by jianw on 2017/12/19 0019.
//

#ifndef INC_01_ASSIMP_MODEL_H
#define INC_01_ASSIMP_MODEL_H

#include "Mesh.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class Model {
public:
    /*  函数   */
    Model(const char* path) {
        loadModel(path);
    }
    void Draw(Shader& shader);
private:
    /*  模型数据  */
    vector<Mesh>    m_meshes;
    string          m_strDirectory;
    /*  函数   */
    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


#endif //INC_01_ASSIMP_MODEL_H
