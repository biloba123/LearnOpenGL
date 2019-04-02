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
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "}";

const char *fShaderSource1 =
    "#version 330 core\n"
    "out vec4 fragColor;\n"
    "void main() {\n"
    "    fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";

const char *fShaderSource2 =
    "#version 330 core\n"
    "out vec4 fragColor;\n"
    "void main() {\n"
    "    fragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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
    GLuint program1 = loadProgram(vShaderSource, fShaderSource1),
        program2 = loadProgram(vShaderSource, fShaderSource2);
    
    GLfloat verticeArrs[][9] = {
        {0.25f, 0.25f, 0.0f,
            0.75f, 0.25f, 0.0f,
            0.5f, 0.75f, 0.0f},
        {-0.25f, -0.25f, -0.0f,
            -0.75f, -0.25f, -0.0f,
            -0.5f, -0.75f, -0.0f}
    };
    
   
    
    GLuint vaos[2], buffers[2];
    glGenVertexArrays(2, vaos);
    glGenBuffers(2, buffers);
    
    for (int i = 0; i < 2; i++) {
        glBindVertexArray(vaos[i]);
        
        //定点缓冲区对象
        glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticeArrs[i]), verticeArrs[i], GL_STATIC_DRAW);
        //定点属性配置
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void *)0);
        glEnableVertexAttribArray(0);
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    //线框模式
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //渲染循环
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(program1);
        glBindVertexArray(vaos[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glUseProgram(program2);
        glBindVertexArray(vaos[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //交换颜色缓冲（双缓冲）
        glfwSwapBuffers(window);
        //检查触发的事件
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(2, vaos);
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
