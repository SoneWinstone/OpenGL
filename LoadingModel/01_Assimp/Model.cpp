//
// Created by jianw on 2017/12/19 0019.
//

#include "Model.h"

void Model::loadModel(const std::string &path) {
    Assimp::Importer importer;
    /**
     * 除了加载文件之外，Assimp允许我们设定一些选项来强制它对导入的数据做一些额外的计算或操作。
     *  aiProcess_Triangulate：如果模型不是（全部）由三角形组成，它需要将模型所有的图元形状变换为三角形。
     *  aiProcess_FlipUVs：在处理的时候翻转y轴的纹理坐标
     *  aiProcess_GenNormals：如果模型不包含法向量的话，就为每个顶点创建法线。
     *  aiProcess_SplitLargeMeshes：将比较大的网格分割成更小的子网格，如果你的渲染有最大顶点数限制，只能渲染较小的网格，那么它会非常有用。
     *  aiProcess_OptimizeMeshes：和上个选项相反，它会将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化。
     */
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);

    // 检查场景和其根节点不为null
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
    m_strDirectory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // 处理节点所有的网格（如果有的话）
    for(unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // 接下来对它的子节点重复这一过程
    for(unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    vector<Vertex>          vertices;
    vector<unsigned int>    indices;
    vector<Texture>         textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // 处理顶点位置、法线和纹理坐标
        //...
        vertices.push_back(vertex);
    }
    // 处理索引
    //...
    // 处理材质
    if(mesh->mMaterialIndex >= 0)
    {
        //...
    }

    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    return vector<Texture>();
}

void Model::Draw(Shader &shader) {
    for (Mesh mesh : m_meshes) {
        mesh.Draw(shader);
    }
}
