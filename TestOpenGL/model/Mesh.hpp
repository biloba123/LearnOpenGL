//
//  Mesh.hpp
//  TestOpenGL
//
//  Created by 吕晴阳 on 2019/4/11.
//  Copyright © 2019 吕晴阳. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Shader.hpp"

using namespace std;
using namespace glm;

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
};

struct Texture {
    GLuint id;
    string type;
    string path;
};

const string TEX_TYPE_DIFFUSE = "texture_diffuse";
const string TEX_TYPE_SPECULAR = "texture_specular";

class Mesh {
public:
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        
        setupMesh();
    }
    
    void Draw(Shader shader) {
        int diffuseNr = 1;
        int specularNr = 1;
        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            string number;
            string name = textures[i].type;
            if (name == TEX_TYPE_DIFFUSE) {
                number = to_string(diffuseNr++);
            } else if (name == TEX_TYPE_SPECULAR) {
                number = to_string(specularNr++);
            }
            shader.setInt(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);
        
        //绘制网格
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    
private:
    GLuint VAO, VBO, EBO;
    
    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
        
        //顶点位置
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
        glEnableVertexAttribArray(0);
        //法线
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        //纹理坐标
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(2);
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};

#endif /* Mesh_hpp */
