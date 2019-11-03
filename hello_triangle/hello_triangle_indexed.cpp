#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//声明窗口回调函数，它会在每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

//声明输入函数
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 600;

//顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
//片段着色器，最后一位代表不透明度
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

int main() {
    //初始化glfw
    glfwInit();
    //这两行是告诉glfw我们使用opengl版本是3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //使用核心模式，OpenGl3.2开始废弃了立即渲染模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //mac系统需要加上这行配置，否则无法编译
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGl", NULL, NULL);
    //nullptr用来区分NULL和0，最好不要用NULL
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        //释放资源
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //注册窗口回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glad用来管理OpenGl函数指针的，所以调用任何OpenGl函数之前都需要初始化glad，参数是加载系统相关
    //的OpenGl函数指针地址
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //创建顶点着色器对象，参数是着色器类型
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //将着色器对象与着色器源码绑定，第二个参数是源码字符串数量，这里只有一个
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    //编辑着色器对象
    glCompileShader(vertexShader);
    //检查编译结果
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    char infoLog[512];
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //创建片段着色器
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //着色器程序将所有着色器链接到一起
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //检查link结果
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }

    //把着色器对象链接到程序对象后，就不再需要它们了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //顶点集合, 使用两个三角形叠加成矩形
    float vertices[] = {
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
    };
    //索引，分别代表顶点集合里的4个坐标
    unsigned int indices[] = {
            0, 1, 3,    //第一个三角形
            1, 2, 3 //第二个三角形
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); //创建顶点数组对象
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO); //绑定数组对象到缓冲

    //顶点缓冲对象绑定到GL_ARRAY_BUFFER，我的理解就是将VBO这个整型指定为顶点缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //将用户定义到数据复制到当前缓冲，即将vertices复制到VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //绑定索引缓冲对象到GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //告诉OpenGl如何解析顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    //启用顶点属性
    glEnableVertexAttribArray(0);

    //解绑VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //解绑VAO
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        //在清除颜色缓冲之后，整个屏幕会被填充为这个颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //清除颜色缓冲
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        //VAO不必每次都绑定，但是这样写比较规范
        glBindVertexArray(VAO);
        //指明从索引缓冲渲染，绘制两个三角形，所以是6个顶点
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //释放资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    //释放GLFW资源
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        //当按下esc时关闭GLFW
        glfwSetWindowShouldClose(window, true);
    }
}