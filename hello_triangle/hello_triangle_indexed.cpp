#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//声明窗口回调函数，它会在每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

//声明输入函数
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        //当按下esc时关闭GLFW
        glfwSetWindowShouldClose(window, true);
    }
}