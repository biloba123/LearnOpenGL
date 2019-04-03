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
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 vertexColor;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "    vertexColor = aColor;\n"
    "}";

const char *fShaderSource =
    "#version 330 core\n"
    "in vec3 vertexColor;\n"
    "out vec4 fragColor;\n"
    "void main() {\n"
    "    fragColor = vec4(vertexColor, 1.0);\n"
    "}\n";

void framebuffersizeCallback(GLFWwindow *window, GLint width, GLint height);
GLuint loadShader(GLenum type, const char *shaderSrc);
GLuint loadProgram(const char *vertShaderSrc, const char *fragShaderSrc);

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
    GLuint program = loadProgram(vShaderSource, fShaderSource);
    
    if (!program) {
        return -1;
    }
    
    GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f,   //vertex
        1.0f, 0.0f, 0.0f,   //color
        0.5f, -0.5f, 0.0f,
        0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f,
        1.0f, 1.0f, 1.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);
    
    //索引缓冲区对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    //线框模式
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //渲染循环
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(program);
        
//        float timeValue = glfwGetTime();
//        float greenValue = sin(timeValue) / 2.0f + 0.5f;
//        GLint location = glGetUniformLocation(program, "ourColor");
//        GLfloat color[] = {0.0f, greenValue, 0.0f, 1.0f};
//        glUniform4fv(location, 1, color);
        
        glBindVertexArray(vao);
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
            char info[infoLen];
            glGetShaderInfoLog(shader, infoLen, NULL, info);
            cout << "ERROR: Shader compilation failed:\n" << info << endl;
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
        
        glDeleteProgram(program);
        return 0;
    }
    
    return program;
}
