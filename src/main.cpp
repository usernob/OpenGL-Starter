// STARTER CODE FROM learnopengl.com

// clang-format off
#include <filesystem>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

void readFile(const char *filename, std::string &output);
unsigned int createShader(const std::string &vertexShader,
                          const std::string &fragmentShader);
unsigned int compileShader(unsigned int type, const std::string &shader);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL test", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << "\n";
    std::cout << "Current directory is: " << std::filesystem::current_path()
              << std::endl;
    std::string vertexShaderSource, fragmentShaderSource;

    // TODO: get PROJECT_SOURCE_DIR value from cmake
    readFile("resource/shaders/vertex.vert.glsl", vertexShaderSource);
    readFile("resource/shaders/fragment.frag.glsl", fragmentShaderSource);
    unsigned int program =
        createShader(vertexShaderSource, fragmentShaderSource);

    if (program == 0) {
        std::cerr << "Failed to create shader" << std::endl;
        return -1;
    }
    // clang-format off
	float vertices1[] = {
		-0.90f, -0.50f, 0.0f,
		-0.50f,  0.50f, 0.0f,
		-0.10f, -0.50f, 0.0f,
    };
    float vertices2[] = {
		 0.90f, -0.50f, 0.0f,
		 0.50f,  0.50f, 0.0f,
		 0.10f, -0.50f, 0.0f
	};
    // clang-format on

    // Vertex Buffer Object
    unsigned int VBO[2], VAO[2];
    glGenBuffers(2, VBO);
    glGenVertexArrays(2, VAO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);



    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse
        // moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlive their purpose
    // ----------------------------------------------------------------------
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(program);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);
}

void readFile(const char *filename, std::string &output) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "file: " << filename << std::endl;
        throw std::runtime_error("Failed to read file");
    }

    std::stringstream ss;
    std::string line;
    while (std::getline(file, line)) {
        ss << line << "\n";
    }

    output = ss.str();
    file.close();
}

unsigned int createShader(const std::string &vertexShader,
                          const std::string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    if (fs == 0 || vs == 0) {
        return 0;
    }

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {

        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        char *message = (char *)alloca(length * sizeof(char));
        glGetProgramInfoLog(program, length, &length, message);

        std::cout << "Failed to link shader\n";
        std::cout << message << std::endl;

        return 0;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int compileShader(unsigned int type, const std::string &shader) {
    unsigned int vertexShader = glCreateShader(type);
    const char *src = shader.c_str();
    glShaderSource(vertexShader, 1, &src, NULL);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {

        int length;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);

        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(vertexShader, length, &length, message);

        std::cout << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader\n";
        std::cout << message << std::endl;

        glDeleteShader(vertexShader);

        return 0;
    }
    return vertexShader;
}
