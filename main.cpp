
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SHADER/shader_s.h>
#include <TEXTURE/texture_s.h>

#include <iostream>

using namespace std;
using namespace glm;

constexpr auto _HEIGHT = 600;
constexpr auto _WIDTH = 800;

bool wireframeOn = false;
mat4 model = mat4(1.0f), view = mat4(1.0f), projection = mat4(1.0f);

#pragma region FPS_COUNTER
double previousTime = 0;
int frameCount = 0;
void fps();
#pragma endregion "Frames per second display"

#pragma region DELTA TIME
float deltaTime = 0.0f;
float timeSinceStart = 0.0f;
void updateDeltaTime();
#pragma endregion "Declaration"

void inputManagement(GLFWwindow* window);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

float FOV = 45.0f;
float swap_var = 0.2f;
float cameraSpeed = 2.5f;

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

    glEnable(GL_DEPTH_TEST);

    #pragma endregion "GLFW window and Glad (OpenGL) setup"

    GLenum texture_config[4] = { GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR };

    Texture container("resources/container.jpg", GL_TEXTURE_2D, 0, texture_config, true);
    Texture face("resources/awesomeface.png", GL_TEXTURE_2D, 1, texture_config, true);

    Shader main_shader("shaders/vShader.vert", "shaders/fShader.frag");


    /*float triangleVertices[] = {
         // positions           // colors           // texture coords
         0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,         // top right
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,         // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,         // bottom left
        -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f          // top left
    };
    
    GLuint indices[] = {
            0, 1, 3,    // first triangle
            1, 2, 3     // second triangle
    };

    */

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    vec3 cubePositions[] = {
        vec3( 0.0f, 0.0f, 0.0f),
        vec3( 2.0f, 5.0f, -15.0f),
        vec3(-1.5f, -2.2f, -2.5f),
        vec3(-3.8f, -2.0f, -12.3f),
        vec3( 2.4f, -0.4f, -3.5f),
        vec3(-1.7f, 3.0f, -7.5f),
        vec3( 1.3f, -2.0f, -2.5f),
        vec3( 1.5f, 2.0f, -2.5f),
        vec3( 1.5f, 0.2f, -1.5f),
        vec3(-1.3f, 1.0f, -1.5f)
    };


    GLuint VAO, VBO;                    // Vertex array object, Vertex buffer object, Element buffer object
    glGenVertexArrays(1, &VAO);         // Generate the buffer array for VAO
    glGenBuffers(1, &VBO);              // Generate the buffer for VBO

/*  
    GLuint EBO;                         
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

    
     //GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
     //GL_STATIC_DRAW: the data is set only once and used many times.
     //GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
*/

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // Unbinding VAO, VBO AND EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    main_shader.use();
    main_shader.setIntUniform("ourTexture0", container.getTextureUnit());
    main_shader.setIntUniform("ourTexture1", face.getTextureUnit());
    
    view = translate(view, vec3(0.0f, 0.0f, -3.0f));
    

    #pragma region MAIN_RENDER_LOOP
    while (!glfwWindowShouldClose(window))
    {
        updateDeltaTime();
        inputManagement(window);

        glClearColor(0.05f, 0.62f, 0.62f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        fps();

        container.bind();
        face.bind();
        main_shader.use();

        projection = perspective(radians(FOV), (float)_WIDTH / (float)_HEIGHT, 0.1f, 100.0f);
        main_shader.setMat4Uniform("view", view);
        main_shader.setMat4Uniform("projection", projection);

        main_shader.setFloatUniform("pick", swap_var);
        
        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++)
        {
            float angle = glfwGetTime() * -1 * i;
            if(i%2==0) angle = glfwGetTime() + i*2;
            
            model = mat4(1.0f);
            model = translate(model, cubePositions[i]);
            model = rotate(model, angle, vec3(1.0f*i, 0.3f*i, 0.5f*i));
            main_shader.setMat4Uniform("model", model);
    
            // Mode, num of vertices, data type of the indices, and offset 
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // Mode, starting index, num of vertices
            glDrawArrays(GL_TRIANGLES, 0, 36); // Without EBO
        }
        glBindVertexArray(0);

        // Double buffer swapping and event catching
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    #pragma endregion "MAIN_RENDER_LOOP"

    // De-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

#pragma region FUNCTIONS IMPLEMENTATION

void fps()
{
    double currentTime = glfwGetTime();
    frameCount++;
    // If a second has passed.
    if (currentTime - previousTime >= 1.0)
    {
        system("cls");
        // Display the frame count here any way you want.
        cout << "FPS: " << frameCount << endl;
        cout << "Delta time: " << deltaTime << endl;
        cout << "FOV: " << FOV << endl;
        frameCount = 0;
        previousTime = currentTime;
    }
}

void updateDeltaTime()
{
	float currentTime = glfwGetTime();
	deltaTime = (currentTime - timeSinceStart);
	timeSinceStart = currentTime;
}

void inputManagement(GLFWwindow* window)
{
    #pragma region ESC_CLOSE_WINDOW
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    #pragma endregion
    #pragma region WIRE_FRAME
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
    #pragma endregion
    #pragma region TEXTURE_SWAP
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        swap_var += deltaTime;
        if(swap_var > 1.0f) swap_var = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        swap_var -= deltaTime;
        if(swap_var < 0.0f) swap_var = 0.0f;
    }
    #pragma endregion
    #pragma region CAMERA_MOVEMENT
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        view = translate(view, vec3(0.0f, 0.0f, cameraSpeed*2*deltaTime));
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        view = translate(view, vec3(0.0f, 0.0f, -cameraSpeed*2*deltaTime));
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        view = translate(view, vec3(0.0f, -cameraSpeed*deltaTime, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        view = translate(view, vec3(0.0f, cameraSpeed*deltaTime, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        view = translate(view, vec3(cameraSpeed*deltaTime, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        view = translate(view, vec3(-cameraSpeed*deltaTime, 0.0f, 0.0f));
    #pragma endregion
    #pragma region FOV
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        FOV += 20.0f*deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        FOV -= 20.0f*deltaTime;
    if (FOV > 100.0f) FOV = 100.0f;
    if (FOV < 45.0f) FOV = 45.0f;
    #pragma endregion
}   

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

#pragma endregion
