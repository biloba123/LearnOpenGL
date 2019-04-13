//
//  Model.hpp
//  TestOpenGL
//
//  Created by 吕晴阳 on 2019/4/11.
//  Copyright © 2019 吕晴阳. All rights reserved.
//

#ifndef Model_hpp
#define Model_hpp

#include "Mesh.hpp"
#include "stb_image.h"
#include "utils.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

class Model
{
public:
    Model(char *path) {
        loadModel(path);
    }
    
    void Draw(Shader shader) {
        for (unsigned int i = 0; i < meshes.size(); i++) {
            meshes[i].Draw(shader);
        }
    }
    
private:
    //模型数据
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;
    
    void loadModel(string path) {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            cout << "ERROR::ASSIMP" << import.GetErrorString() << endl;
            return;
        }
        
        directory = path.substr(0, path.find_last_of('/'));
        
        processNode(scene->mRootNode, scene);
    }
    
    void processNode(aiNode *node, const aiScene *scene) {
        for(unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        
        for(unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }
    
    Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
        vector<Vertex> vertices;
        vector<GLuint> indices;
        vector<Texture> textures;
        
        
        aiVector3D *texCoords = mesh->mTextureCoords[0];
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            
            aiVector3D position = mesh->mVertices[i];
            vertex.position = glm::vec3(position.x, position.y, position.z);
            aiVector3D normal = mesh->mNormals[i];
            vertex.normal = glm::vec3(normal.x, normal.y, normal.z);
            if (texCoords) {
                aiVector3D texCoord = texCoords[i];
                vertex.texCoord = glm::vec2(texCoord.x, texCoord.y);
            } else {
                vertex.texCoord = glm::vec2(0.0f);
            }
            
            vertices.push_back(vertex);
        }
        
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
        }
        
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TEX_TYPE_DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TEX_TYPE_SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
        
        return Mesh(vertices, indices, textures);
    }
    
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         string typeName) {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for(unsigned int j = 0; j < textures_loaded.size(); j++) {
                if (strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0) {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            
            if (!skip) {
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
        
        return textures;
    }
};

#endif /* Model_hpp */
