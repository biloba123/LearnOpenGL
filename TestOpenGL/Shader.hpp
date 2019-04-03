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

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
public:
    GLuint ID;
    
    Shader(const char *vertexPath, const char *fragmentPath) {
        string vertexCode, fragmentCode;
        
        try {
            vertexCode = readContent(vertexPath);
            fragmentCode = readContent(fragmentPath);
        } catch (ifstream::failure e) {
            cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        }
        
        ID = loadProgram(vertexCode.c_str(), fragmentCode.c_str());
    }
    
    void use() {
        glUseProgram(ID);
    }
    
private:
    string readContent(const char *path) {
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
    
    GLuint loadProgram(const char *vertShaderSrc, const char *fragShaderSrc) {
        GLuint vShader = loadShader(GL_VERTEX_SHADER, vertShaderSrc);
        GLuint fShader = loadShader(GL_FRAGMENT_SHADER, fragShaderSrc);
        
        if (!vShader || !fShader) {
            return 0;
        }
        
        GLuint program = glCreateProgram();
        glAttachShader(program, vShader);
        glAttachShader(program, fShader);
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
            glDeleteProgram(program);
            return 0;
        }
        
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        return program;
    }

};

#endif /* Shader_hpp */
