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
#include "Camera.hpp"
#include "stb_image_wrapper.h"
using namespace std;
using namespace glm;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char *PROJECT_ROOT = "/Users/lvqingyang/Projects_Xcode/TestOpenGL/TestOpenGL";

float mixValue = 0.2f;

//Camera
Camera camera(vec3(0.0f, 1.0f, 5.0f));
float lastFrame = 0.0f; //上一帧的时间
float deltaTime = 0.0f; //当前帧与上一帧时间差
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

bool isKeyPressed(GLFWwindow *window, int key);
void processInput(GLFWwindow *window);
void framebuffersizeCallback(GLFWwindow *window, GLint width, GLint height);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
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
    char *vsPath = getAbsolutePath("/resource/colors.vs"), *fsPath = getAbsolutePath("/resource/colors.fs");
    Shader lightingShader(vsPath, fsPath);
    free(vsPath);
    free(fsPath);
    
    vsPath = getAbsolutePath("/resource/lamp.vs");
    fsPath = getAbsolutePath("/resource/lamp.fs");
    Shader lampShader(vsPath, fsPath);
    free(vsPath);
    free(fsPath);
    
    
    if (!lightingShader.ID || !lampShader.ID) {
        return -1;
    }
    
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
    };
    
    GLubyte elements[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20,
    };
    
    
    GLuint cuboVAO, buffers[2];
    glGenVertexArrays(1, &cuboVAO);
    glGenBuffers(2, buffers);
    glBindVertexArray(cuboVAO);

    //顶点缓冲区对象
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //顶点属性配置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);
    
    //索引缓冲区对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    
    
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    
    glBindVertexArray(0);
   
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    lightingShader.use();
    lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    //渲染循环
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        lightPos.x = sin(currentFrame) + 1.2f;
        lightPos.y = sin(currentFrame / 2) + 1.0f;
        
        mat4 model = mat4(1.0f);
        mat4 view = camera.getViewMatrix();
        mat4 projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        
        lightingShader.use();
        lightingShader.setMat4("model", model);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setVec3("lightPos", lightPos);
        glBindVertexArray(cuboVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)0);
        
        model = mat4(1.0f);
        model = translate(model, lightPos);
        model = scale(model, glm::vec3(0.2f));
        
        lampShader.use();
        lampShader.setMat4("model", model);
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);
        glBindVertexArray(lightVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)0);
        
        //交换颜色缓冲（双缓冲）
        glfwSwapBuffers(window);
        //检查触发的事件
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &cuboVAO);
    glDeleteVertexArrays(1, &lightVAO);
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
