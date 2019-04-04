//
//  main.cpp
//  TestOpenGL
//
//  Created by 吕晴阳 on 2019/4/1.
//  Copyright © 2019 吕晴阳. All rights reserved.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "Shader.hpp"
#include "stb_image_wrapper.h"
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *PROJECT_ROOT = "/Users/lvqingyang/Projects_Xcode/TestOpenGL/TestOpenGL";


void framebuffersizeCallback(GLFWwindow *window, GLint width, GLint height);
char *getAbsolutePath(const char *relativePath);

int main() {
    //初始化GLFW
    glfwInit();
    
    //配置GLFW
    //指定OPenGL版本3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    //创建窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (!window) {
        cout << "Failed to creat GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    //设置window的上下文为当前现场上下文
    glfwMakeContextCurrent(window);
    
    //初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to init GLAD" << endl;
        return -1;
    }
    
    //设置渲染窗口（视口）
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    //设置窗口大小改变回调
    glfwSetFramebufferSizeCallback(window, framebuffersizeCallback);
    
    
    //创建链接着色器程序对象
    char *vsPath = getAbsolutePath("/shader.vs"), *fsPath = getAbsolutePath("/shader.fs");
    Shader shaderProgram(vsPath, fsPath);
    free(vsPath);
    free(fsPath);
    
   
    if (!shaderProgram.ID) {
        return -1;
    }
    
    GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f,   //vertex
        1.0f, 0.0f, 0.0f,   //color
        1.0f, 1.0f,          //texture coordinate
        0.5f, -0.5f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f
    };
    
    GLubyte elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint vao, buffers[2];
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, buffers);
    
    glBindVertexArray(vao);
    
    //顶点缓冲区对象
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //顶点属性配置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 6));
    glEnableVertexAttribArray(2);
    
    //索引缓冲区对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //生成纹理
    GLuint textures[2];
    const char *imgPaths[] = {"/container.jpg", "/awesomeface.png"};
    GLenum imgTypes[] = {GL_RGB, GL_RGBA};
    glGenTextures(2, textures);
    stbi_set_flip_vertically_on_load(true);
    for (int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        //设置纹理对象参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //加载并设置纹理图像
        char *absoluteImgPath = getAbsolutePath(imgPaths[i]);
        GLint width, height, comp;
        unsigned char *data = stbi_load(absoluteImgPath, &width, &height, &comp, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, imgTypes[i], width, height, 0, imgTypes[i], GL_UNSIGNED_BYTE, data);
            //多级渐进纹理
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            cout << "Failed to load texture" << endl;
        }
        stbi_image_free(data);
        free(absoluteImgPath);
    }
    
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    shaderProgram.use();
    shaderProgram.setIntUniform("ourTexture1", 0);
    shaderProgram.setIntUniform("ourTexture2", 1);
    
    //线框模式
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //渲染循环
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        shaderProgram.use();
        glBindVertexArray(vao);
        for (int i = 0; i < 2; i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
        }
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void *)0);
        
        //交换颜色缓冲（双缓冲）
        glfwSwapBuffers(window);
        //检查触发的事件
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(2, buffers);
    
    //释放资源
    glfwTerminate();
    return 0;
    
}

void framebuffersizeCallback(GLFWwindow *window, GLint width, GLint height) {
    glViewport(0, 0, width, height);
}

char *getAbsolutePath(const char *relativePath) {
    char *absolutePath = (char *)malloc(strlen(PROJECT_ROOT) + strlen(relativePath) + 1);
    
    strcpy(absolutePath, PROJECT_ROOT);
    strcat(absolutePath, relativePath);
    
    return absolutePath;
}
