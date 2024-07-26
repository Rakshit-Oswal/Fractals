//iterations will determine how DETAILED the fractal is
//high iteration = slower rendering

#include "shaders.cpp"
#include <fstream>
#include <sstream>
#include <vector>
#define COLOR(colourValue) colourValue/255.0f 

int Height = 1000, Width = 800, iterations = 1000;
float lastFrame = 0.0f, deltaTime = .0f, offsetX = 0.0, offsetY = 0.0, zoom = 100.0;

std::vector<float> vertices = {
    -1.0, -1.0,
     1.0,  1.0,
    -1.0,  1.0,
     1.0, -1.0  
};  

std::vector<int> indices = {
    0, 3, 1,
    1, 2, 0
};

void setBuffers(){
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open the file - '" << filePath << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

GLFWwindow* InitWindow(){
    if (!glfwInit()){
        std::cout << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(Width, Height, "OpenGL-Fractal",NULL,NULL);
    glfwSetWindowPos(window, 300, 100);

    if(!window) std::cout<<"Window was not created";
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }
    return window;
}

void pollKeyboardInputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        offsetX += 20 / zoom;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        offsetX -= 20 / zoom;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        offsetY -= 20 / zoom;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        offsetY += 20 / zoom;
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        zoom *= 1.2;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        zoom /= 1.2;
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        iterations += 20;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        iterations = (iterations > 100) ? iterations - 20 : 20;
}


int main() {
    GLFWwindow* window = InitWindow();  
    if(!window) return -1;
    
    glViewport(0, 0, Width, Height);

    setBuffers();

    std::string vertexShader = readFile("shader.vert"), fragmentShader = readFile("shader.frag");
    GLuint shader = CreateShader(vertexShader, fragmentShader);

    glEnable(GL_DEPTH_TEST);
    // Blending used to allow transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //-------------
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Update
        pollKeyboardInputs(window);

        //Draw
        glClearColor(COLOR(245),COLOR(245),COLOR(245), 1.0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        int viewportWidth, viewportHeight;
        glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
        glUniform2f(glGetUniformLocation(shader, "offset"), (float)offsetX, (float)offsetY);
        glUniform2f(glGetUniformLocation(shader, "viewportSize"), (float)viewportWidth, (float)viewportHeight);
        glUniform1i(glGetUniformLocation(shader, "iterations"), iterations);
        glUniform1f(glGetUniformLocation(shader, "zoom"), (float)zoom);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
