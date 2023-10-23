
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SHADER/shader_s.h>

#include <iostream>

using namespace std;

constexpr auto _HEIGHT = 800;
constexpr auto _WIDTH = 600;

bool wireframeOn = false;

#pragma region FPS_COUNTER
double previousTime = glfwGetTime();
int frameCount = 0;
void fps()
{
    double currentTime = glfwGetTime();
    frameCount++;
    // If a second has passed.
    if (currentTime - previousTime >= 1.0)
    {
        // Display the frame count here any way you want.
        cout << frameCount << endl;

        frameCount = 0;
        previousTime = currentTime;
    }
}
#pragma endregion "Frames per second display"

void inputManagement(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        if (wireframeOn) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            wireframeOn = false;
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            wireframeOn = true;
        }
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    #pragma region SETUP

    // GLFW initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(_WIDTH, _HEIGHT, "Test", NULL, NULL);

    // Window init
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Define and init render window and rescaling
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Try load Glad for his own OS-specific pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to inicialice Glad" << endl;
        glfwTerminate();
        return -1;
    }

    #pragma endregion "GLFW window and Glad (OpenGL) setup"

    Shader main_shader = Shader("shaders/vShader.vert", "shaders/fShader.frag");

    float triangleVertices[] = {
        // Positions          // Colors
         0.5f,  0.5f,  0.0f,  1.0f, 0.0f, 0.0f,    // top right
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,    // bottom right
        -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f,    // bottom left
        -0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 0.5f     // top left
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,                // first triangle
        1, 2, 3                 // second triangle
    };

    unsigned int VAO, VBO, EBO;         // Vertex array object, Vertex buffer object, Element buffer object
    glGenVertexArrays(1, &VAO);         // Generate the buffer array for VAO
    glGenBuffers(1, &VBO);              // Generate the buffer for VBO
    glGenBuffers(1, &EBO);              // Generate the buffer for EBO

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes
    glBindVertexArray(VAO);

    // Bind a specific target to our just created buffer, now any call to GL_ARRAY_BUFFER refers to VBO
    // Copy user data to target VBO buffer in graphics card memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    // Same for EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /*
     GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
     GL_STATIC_DRAW: the data is set only once and used many times.
     GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    */

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    // Unbinding VAO, VBO AND EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    #pragma region MAIN_RENDER_LOOP
    while (!glfwWindowShouldClose(window))
    {
        inputManagement(window);

        glClearColor(0.1f, 0.42f, 0.52f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        fps();

        main_shader.use();

        glBindVertexArray(VAO);
        // Mode, num of vertices, data type of the indices, and offset 
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Mode, starting index, num of vertices
        // glDrawArrays(GL_TRIANGLES, 0, 6); // Without EBO
        glBindVertexArray(0);

        // Double buffer swapping and event catching
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    #pragma endregion "MAIN_RENDER_LOOP"

    // De-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}