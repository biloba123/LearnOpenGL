//
//  main.cpp
//  AdvancedOpenGL
//
//  Created by 吕晴阳 on 2019/4/13.
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
#include "Camera.hpp"
#include "stb_image_wrapper.h"
using namespace std;
using namespace glm;

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const char *PROJECT_ROOT = "/Users/lvqingyang/Projects_Xcode/TestOpenGL/AdvancedOpenGL";

//Camera
Camera camera(vec3(0.0f, 0.0f, 3.0f));
float lastFrame = 0.0f; //上一帧的时间
float deltaTime = 0.0f; //当前帧与上一帧时间差
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

bool isKeyPressed(GLFWwindow *window, int key);
void processInput(GLFWwindow *window);
void framebuffersizeCallback(GLFWwindow *window, GLint width, GLint height);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
char *getAbsolutePath(const char *relativePath);
GLuint loadTexture(const char *relativePath);

#define NR_POINT_LIGHTS 4

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "AdvancedOpenGL", NULL, NULL);
    if (!window) {
        cout << "Failed to creat GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    //设置window的上下文为当前现场上下文
    glfwMakeContextCurrent(window);
    
    
    //设置窗口大小改变回调
    glfwSetFramebufferSizeCallback(window, framebuffersizeCallback);
    //光标位置回调
    glfwSetCursorPosCallback(window, mouseCallback);
    //鼠标滚轮回调
    glfwSetScrollCallback(window, scrollCallback);
    
    //捕捉光标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    //初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to init GLAD" << endl;
        return -1;
    }
    
    
    //创建链接着色器程序对象
    char *vsPath = getAbsolutePath("/resource/scene.vs"), *fsPath = getAbsolutePath("/resource/scene.fs");
    Shader shader(vsPath, fsPath);
    free(vsPath);
    free(fsPath);
    
    vsPath = getAbsolutePath("/resource/single_color.vs");
    fsPath = getAbsolutePath("/resource/single_color.fs");
    Shader singleColorShader(vsPath, fsPath);
    free(vsPath);
    free(fsPath);
    
    
    if (!shader.ID) {
        return -1;
    }
    
    GLfloat cubeVertices[] = {
        // positions       // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    vec3 cuboPos[] = {
        vec3(-1.0f, 0.0f, -1.0f),
        vec3(2.0f, 0.0f, 0.0f),
    };
    
    float planeVertices[] = {
        // positions       // texture Coords
        5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
        
        5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
        5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    
    
    //cubo VAO
    GLuint cuboVAO, cuboVBO;
    glGenVertexArrays(1, &cuboVAO);
    glGenBuffers(1, &cuboVBO);
    glBindVertexArray(cuboVAO);
    //顶点缓冲区对象
    glBindBuffer(GL_ARRAY_BUFFER, cuboVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    //顶点属性配置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid *)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    //plane VAO
    GLuint planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid *)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    GLuint cuboTexture = loadTexture("/resource/marble.jpg");
    GLuint planeTexture = loadTexture("/resource/metal.png");
    
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //深度测试
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //模版测试
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //渲染循环
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        mat4 model = mat4(1.0f);
        mat4 view = camera.getViewMatrix();
        mat4 projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        
        //plane
        glStencilFunc(GL_ALWAYS, 1, 0XFF);
        glStencilMask(0x00);
        shader.use();
        model = translate(model, vec3(0.0f, -0.01f, 0.0f));
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, planeTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //cubo
        glStencilFunc(GL_ALWAYS, 1, 0XFF);
        glStencilMask(0xFF);
        glBindVertexArray(cuboVAO);
        glBindTexture(GL_TEXTURE_2D, cuboTexture);
        for (int i = 0; i < 2; i++) {
            model = mat4(1.0f);
            model = translate(model, cuboPos[i]);
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        //single color
        glStencilFunc(GL_NOTEQUAL, 1, 0XFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        singleColorShader.use();
        singleColorShader.setMat4("view", view);
        singleColorShader.setMat4("projection", projection);
        glBindVertexArray(cuboVAO);
        for (int i = 0; i < 2; i++) {
            model = mat4(1.0f);
            model = translate(model, cuboPos[i]);
            model = scale(model, vec3(1.05f));
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glEnable(GL_DEPTH_TEST);
        glStencilMask(0xFF);
        
        //交换颜色缓冲（双缓冲）
        glfwSwapBuffers(window);
        //检查触发的事件
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &cuboVAO);
    glDeleteVertexArrays(1, &planeVAO);
    
    //释放资源
    glfwTerminate();
    return 0;
    
}

void processInput(GLFWwindow *window) {
    if (isKeyPressed(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
    
    if (isKeyPressed(window, GLFW_KEY_W)) {
        camera.processKeyboard(FORWARD, deltaTime);
    }
    
    if (isKeyPressed(window, GLFW_KEY_S)) {
        camera.processKeyboard(BACKWARD, deltaTime);
    }
    
    if (isKeyPressed(window, GLFW_KEY_A)) {
        camera.processKeyboard(LEFT, deltaTime);
    }
    
    if (isKeyPressed(window, GLFW_KEY_D)) {
        camera.processKeyboard(RIGHT, deltaTime);
    }
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    static bool firstMouse = true;
    if (firstMouse) {
        firstMouse = false;
        lastX = xpos;
        lastY = ypos;
    }
    
    float xOffset = xpos - lastX, yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    camera.processMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.processMouseScroll(yoffset);
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

GLuint loadTexture(const char *relativePath) {
    GLuint textureID;
    char *path = getAbsolutePath(relativePath);
    glGenTextures(1, &textureID);
    
    int width = 0, height = 0, comp = 0;
    unsigned char *data = stbi_load(path, &width, &height, &comp, 0);
    if (data) {
        GLenum format;
        switch (comp) {
            case 1: format = GL_RED;    break;
            case 3: format = GL_RGB;    break;
            case 4: format = GL_RGBA;   break;
            default: break;
        }
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        cout << "Texture failed to load at path: " << path << endl;
    }
    
    stbi_image_free(data);
    free(path);
    return textureID;
}
