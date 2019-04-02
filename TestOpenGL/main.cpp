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

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffersizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

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
        std::cout << "Failed to creat GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //设置window的上下文为当前现场上下文
    glfwMakeContextCurrent(window);
    
    //初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }
    
    //设置渲染窗口（视口）
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    //设置窗口大小改变回调
    glfwSetFramebufferSizeCallback(window, framebuffersizeCallback);
    
    //渲染循环
    while (!glfwWindowShouldClose(window)) {
        //交换颜色缓冲（双缓冲）
        glfwSwapBuffers(window);
        //检查触发的事件
        glfwPollEvents();
    }
    
    //释放资源
    glfwTerminate();
    return 0;
    
}
