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
#include "model/Model.hpp"
#include "stb_image_wrapper.h"
using namespace std;
using namespace glm;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char *PROJECT_ROOT = "/Users/lvqingyang/Projects_Xcode/TestOpenGL/TestOpenGL";

float mixValue = 0.2f;

//Camera
Camera camera(vec3(0.0f, 1.0f, 4.0f));
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
    char *vsPath = getAbsolutePath("/resource/model_loading.vs"), *fsPath = getAbsolutePath("/resource/model_loading.fs");
    Shader ourShader(vsPath, fsPath);
    free(vsPath);
    free(fsPath);
    
    if (!ourShader.ID) {
        return -1;
    }
    
    Model ourModel("/Users/lvqingyang/Projects_OpenGL/nanosuit/nanosuit.obj");
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    ourShader.use();
    ourShader.setVec3("spotLight.ambient", vec3(0.2f));
    ourShader.setVec3("spotLight.diffuse", vec3(0.5f));
    ourShader.setVec3("spotLight.specular", vec3(1.0f));
    ourShader.setFloat("spotLight.cutOff", cos(radians(12.5f)));
    ourShader.setFloat("spotLight.outerCutOff", cos(radians(17.5f)));
    ourShader.setFloat("spotLight.constant", 1.0f);
    ourShader.setFloat("spotLight.linear", 0.09f);
    ourShader.setFloat("spotLight.quadratic", 0.032f);
    ourShader.setFloat("material.shininess", 32.0f);
    //渲染循环
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ourShader.use();
        ourShader.setVec3("spotLight.position", camera.Position);
        ourShader.setVec3("spotLight.direction", camera.Front);
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        
        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));    // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);
        
        //交换颜色缓冲（双缓冲）
        glfwSwapBuffers(window);
        //检查触发的事件
        glfwPollEvents();
    }
    
    
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
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    
    if (isKeyPressed(window, GLFW_KEY_S)) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    
    if (isKeyPressed(window, GLFW_KEY_A)) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    
    if (isKeyPressed(window, GLFW_KEY_D)) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
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
    
    camera.ProcessMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
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
