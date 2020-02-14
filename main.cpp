#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

int mainLast() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //创建窗口
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //初始化glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    //注册窗口变化函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //OpenGl渲染窗口大小，前两个参数为窗口左下角位置，后两个为宽高
    glViewport(0, 0, 800, 600);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        //清空颜色缓冲后的填充色
        glClearColor(0.2f, 0.3f, 0.3f, 0.5f);
        //清空屏幕颜色缓冲
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        //检查触发事件(键盘输入，鼠标移动)
        glfwPollEvents();
    }

    cout << "close window" << endl;
    //释放之前分配的所有资源
    glfwTerminate();
    return 0;
}

/**
 * 窗口大小改变时，视口也要改变
 */
void framebuffer_size_callbackLast(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

/**
 * 检查用户输入
 * @param window
 */
void processInputLast(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        //当按下esc时关闭GLFW
        glfwSetWindowShouldClose(window, true);
    }
}