//
//  Shader.hpp
//  TestOpenGL
//
//  Created by 吕晴阳 on 2019/4/3.
//  Copyright © 2019 吕晴阳. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
public:
    GLuint ID;
    
    Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = NULL) {
        string vertexCode, fragmentCode, geometryCode;
        
        try {
            vertexCode = readContent(vertexPath);
            fragmentCode = readContent(fragmentPath);
            geometryCode = readContent(geometryPath);
        } catch (ifstream::failure e) {
            cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        }
        
        ID = loadProgram(vertexCode.c_str(), fragmentCode.c_str(), geometryCode.empty() ? NULL : geometryCode.c_str());
    }
    
    void use() {
        glUseProgram(ID);
    }
    
    void setInt(const char *name, GLint value) {
        glUniform1i(glGetUniformLocation(ID, name), value);
    }
    
    void setFloat(const char *name, GLfloat value) {
        glUniform1f(glGetUniformLocation(ID, name), value);
    }
    
    void setMat4(const char *name, glm::mat4 mat) {
        glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat));
    }
    
    void setVec3(const char *name, GLfloat x, GLfloat y, GLfloat z) {
        glUniform3f(glGetUniformLocation(ID, name), x, y, z);
    }
    
    void setVec3(const char *name, glm::vec3 vec) {
        glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec));
    }
    
    void setBindpoint(const char *blockname, GLuint bindingPoint) {
        GLuint index = glGetUniformBlockIndex(ID, blockname);
        glUniformBlockBinding(ID, index, bindingPoint);
    }
    
private:
    string readContent(const char *path) {
        if (!path) return "";
        
        string content;
        ifstream file;
        
        //确保ifstream能抛出异常
        file.exceptions(ifstream::failbit | ifstream::badbit);
        //打开文件
        file.open(path);
        //读入文件buffer到流中
        stringstream strStream;
        strStream << file.rdbuf();
        //关闭文件
        file.close();
        //将流转换为string
        content = strStream.str();
        
        return content;
    }
    
    GLuint loadShader (GLenum type, const char *shaderSrc) {
        if (!shaderSrc) return 0;
        
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &shaderSrc, NULL);
        glCompileShader(shader);
        
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                char infoLog[infoLen];
                glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
                cout << "ERROR::SHADER_COMPILATION_ERROR of type:" << infoLog << endl;
            }
            
            glDeleteShader(shader);
            return 0;
        }
        
        return shader;
    }
    
    GLuint loadProgram(const char *vertShaderSrc, const char *fragShaderSrc, const char *geomShaderSrc) {
        GLuint vShader = loadShader(GL_VERTEX_SHADER, vertShaderSrc);
        GLuint fShader = loadShader(GL_FRAGMENT_SHADER, fragShaderSrc);
        GLuint gShader = loadShader(GL_GEOMETRY_SHADER, geomShaderSrc);
        
        if (!vShader || !fShader) {
            return 0;
        }
        
        GLuint program = glCreateProgram();
        glAttachShader(program, vShader);
        glAttachShader(program, fShader);
        if (gShader) glAttachShader(program, gShader);
        glLinkProgram(program);
        
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            GLint infoLen = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                char info[infoLen];
                glGetProgramInfoLog(program, infoLen, NULL, info);
                cout << "ERROR: Program linking failed:\n" << info << endl;
            }
            
            glDeleteShader(vShader);
            glDeleteShader(fShader);
            if (gShader) glDeleteShader(gShader);
            glDeleteProgram(program);
            return 0;
        }
        
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        if (gShader) glDeleteShader(gShader);
        return program;
    }

};

#endif /* Shader_hpp */
