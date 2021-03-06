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
    char *vsPath = getAbsolutePath("/resource/multiple_lights.vs"), *fsPath = getAbsolutePath("/resource/multiple_lights.fs");
    Shader lightingShader(vsPath, fsPath);
    free(vsPath);
    free(fsPath);
    
    vsPath = getAbsolutePath("/resource/lamp.vs");
    fsPath = getAbsolutePath("/resource/lamp.fs");
    Shader lampShader(vsPath, fsPath);
    free(vsPath);
    free(fsPath);
    
    vsPath = getAbsolutePath("/resource/multiple_lights.vs");
    fsPath = getAbsolutePath("/resource/single_color.fs");
    Shader singleColorShader(vsPath, fsPath);
    free(vsPath);
    free(fsPath);
    
    
    if (!lightingShader.ID || !lampShader.ID || !singleColorShader.ID) {
        return -1;
    }
    
    GLfloat vertices[] = {
        //positions        //normals        //texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
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
    
    glm::vec3 pointLightPositions[NR_POINT_LIGHTS] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    
    glm::vec3 pointLightColors[NR_POINT_LIGHTS] = {
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.3f, 0.1f, 0.1f)
    };
    
    
    GLuint cuboVAO, buffers[2];
    glGenVertexArrays(1, &cuboVAO);
    glGenBuffers(2, buffers);
    glBindVertexArray(cuboVAO);

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
    
    
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    
    glBindVertexArray(0);
    
    //漫反射贴图
    GLuint diffuseMap = loadTexture("/resource/container2.png");
    GLuint specularMap = loadTexture("/resource/container2_specular.png");
    
    lightingShader.use();
    //漫反射贴图
    lightingShader.setInt("material.diffuse", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    //镜面光贴图
    lightingShader.setInt("material.specular", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    lightingShader.setFloat("material.shininess", 64.0f);
    //定向光
    lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    lightingShader.setVec3("dirLight.ambient", vec3(0.1f));
    lightingShader.setVec3("dirLight.diffuse", vec3(0.2f));
    lightingShader.setVec3("dirLight.specular", vec3(0.5f));
    //点光源
    vec3 pointLightAttrs[NR_POINT_LIGHTS * 5] = {
        //(constant, linear, quadratic) //ambient         //diffuse             //specular
        vec3(1.0f, 0.09f, 0.032f),  pointLightColors[0] * 0.1f, pointLightColors[0], pointLightColors[0],
        vec3(1.0f, 0.09f, 0.032f),  pointLightColors[1] * 0.1f, pointLightColors[1], pointLightColors[1],
        vec3(1.0f, 0.09f, 0.032f),  pointLightColors[2] * 0.1f, pointLightColors[2], pointLightColors[2],
        vec3(1.0f, 0.09f, 0.032f),  pointLightColors[3] * 0.1f, pointLightColors[3], pointLightColors[3],
        
    };
    for (unsigned int i = 0; i < NR_POINT_LIGHTS; i++) {
        string pointLight = "pointLights[" + to_string(i) + "].";
        lightingShader.setVec3((pointLight + "position").c_str(), pointLightPositions[i]);
        vec3 attenuation = pointLightAttrs[i * 4];
        lightingShader.setFloat((pointLight + "constant").c_str(), attenuation.x);
        lightingShader.setFloat((pointLight + "linear").c_str(), attenuation.y);
        lightingShader.setFloat((pointLight + "quadratic").c_str(), attenuation.z);
        lightingShader.setVec3((pointLight + "ambient").c_str(), pointLightAttrs[i * 4 + 1]);
        lightingShader.setVec3((pointLight + "diffuse").c_str(), pointLightAttrs[i * 4 + 2]);
        lightingShader.setVec3((pointLight + "specular").c_str(), pointLightAttrs[i * 4 + 3]);
    }
    //聚光灯
    lightingShader.setVec3("spotLight.ambient", vec3(0.0f));
    lightingShader.setVec3("spotLight.diffuse", vec3(1.0f));
    lightingShader.setVec3("spotLight.specular", vec3(1.0f));
    lightingShader.setFloat("spotLight.constant", 1.0f);
    lightingShader.setFloat("spotLight.linear", 0.09f);
    lightingShader.setFloat("spotLight.quadratic", 0.032f);
    lightingShader.setFloat("spotLight.cutOff", cos(radians(10.0f)));
    lightingShader.setFloat("spotLight.outerCutOff", cos(radians(15.0f)));
    
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
        
        mat4 view = camera.getViewMatrix();
        mat4 projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        lightingShader.use();
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        glBindVertexArray(cuboVAO);
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);
            
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)0);
        }
        
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        singleColorShader.use();
        singleColorShader.setMat4("view", view);
        singleColorShader.setMat4("projection", projection);
        singleColorShader.setVec3("viewPos", camera.Position);
        singleColorShader.setVec3("spotLight.position", camera.Position);
        singleColorShader.setVec3("spotLight.direction", camera.Front);
        glBindVertexArray(cuboVAO);
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            model = scale(model, vec3(1.05f));
            singleColorShader.setMat4("model", model);
            
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)0);
        }
        
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        lampShader.use();
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);
        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < NR_POINT_LIGHTS; i++) {
            mat4 model = mat4(1.0f);
            model = translate(model, pointLightPositions[i]);
            model = scale(model, vec3(0.2f));
            lampShader.setMat4("model", model);
            lampShader.setVec3("color", pointLightColors[i]);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)0);
        }
        //Don't froget!!!
        glStencilMask(0xFF);
        
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
