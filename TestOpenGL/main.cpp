//
//  main.cpp
//  TestOpenGL
//
//  Created by 吕晴阳 on 2019/4/1.
//  Copyright © 2019 吕晴阳. All rights reserved.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "Shader.hpp"
#include "stb_image_wrapper.h"
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char *PROJECT_ROOT = "/Users/lvqingyang/Projects_Xcode/TestOpenGL/TestOpenGL";

float mixValue = 0.2f;

bool isKeyPressed(GLFWwindow *window, int key);
void processInput(GLFWwindow *window);
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
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,//0, 1, 2, 2, 3, 0
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,//4, 5, 6, 6, 7, 4
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,//8, 9, 10, 10, 11, 8
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    };
    
        GLubyte elements[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            8, 9, 10, 10, 11, 8,
            12, 13, 14, 14, 15, 12,
            16, 17, 18, 18, 19, 16,
            20, 21, 22, 22, 23, 20,
        };
    
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    GLuint vao, buffers[2];
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, buffers);
    
    glBindVertexArray(vao);
    
    //顶点缓冲区对象
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //顶点属性配置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);
    
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
    shaderProgram.setInt("ourTexture1", 0);
    shaderProgram.setInt("ourTexture2", 1);
    
    //线框模式
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    
    //渲染循环
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //构造变化矩阵
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        
        
        shaderProgram.use();
        glBindVertexArray(vao);
        for (int i = 0; i < 2; i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
        }
        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("projection", projection);
        shaderProgram.setFloat("mixValue", mixValue);
        
        for (int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            if (i % 3 == 0) {
                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            }
            shaderProgram.setMat4("model", model);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)0);
        }
        
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

void processInput(GLFWwindow *window) {
    if (isKeyPressed(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
    
    if (isKeyPressed(window, GLFW_KEY_UP)) {
        mixValue += 0.001f;
        if (mixValue > 1.0f) {
            mixValue = 1.0f;
        }
    }
    
    if (isKeyPressed(window, GLFW_KEY_DOWN)) {
        mixValue -= 0.001f;
        if (mixValue < 0.0f) {
            mixValue = 0.0f;
        }
    }
}

bool isKeyPressed(GLFWwindow *window, int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
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
