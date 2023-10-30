
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SHADER/shader_s.hpp>
#include <TEXTURE/texture_s.hpp>
#include <CAMERA/base_camera.hpp>

#include <iostream>


#define _HEIGHT 600
#define _WIDTH 800

#pragma region CAMERA VARIABLES
BaseCamera camera(glm::vec3(-2.0f, 1.0f, 6.0f));

bool wireframeOn = false, windowFocus = true;
glm::mat4 model = glm::mat4(1.0f), view = glm::mat4(1.0f), projection = glm::mat4(1.0f);
float FOV = 45.0f;
float swap_var = 0.2f;
float cameraSpeed = 5.0f;
#pragma endregion

#pragma region EXT FUNCTIONS VARIABLES
double previousTime = 0;
int frameCount = 0;
float deltaTime = 0.0f;
float timeSinceStart = 0.0f;
#pragma endregion

#pragma region EXT FUNCTIONS DECLARATION
void fps();
void updateDeltaTime();
void inputManagement(GLFWwindow* window);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
#pragma endregion
#pragma region CALLBACKS DECLARATION
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void mouse_scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
#pragma endregion


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
        std::cout << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Define and init render window and rescaling
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Try load Glad for his own OS-specific pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to inicialice Glad" << '\n';
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);

    #pragma endregion "GLFW window and Glad (OpenGL) setup"

    GLenum texture_config[4] = { GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR };

    Texture container("resources/textures/container.jpg", GL_TEXTURE_2D, 0, texture_config);
    Texture face("resources/textures/awesomeface.png", GL_TEXTURE_2D, 1, texture_config);

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

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
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
    

    #pragma region MAIN_RENDER_LOOP
    while (!glfwWindowShouldClose(window))
    {
        updateDeltaTime();
        inputManagement(window);
        
        glClearColor(0.05f, 0.62f, 0.62f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        fps();

        camera.setCamSpeed(cameraSpeed);
        if(camera.getPosition().y < -3.0f) camera.setPositionY(-3.0f);

        container.bind();
        face.bind();
        main_shader.use();

        view = camera.getViewMatrix();
        projection = glm::perspective(glm::radians(FOV), (float)_WIDTH / (float)_HEIGHT, 0.1f, 100.0f);

        main_shader.setMat4Uniform("view", view);
        main_shader.setMat4Uniform("projection", projection);

        main_shader.setFloatUniform("pick", swap_var);
        
        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++)
        {
            float angle = glfwGetTime() * -1 * i;
            if (i % 2 == 0) angle = glfwGetTime() + i * 2;
            
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, angle, glm::vec3(1.0f*i, 0.3f*i, 0.5f*i));
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

#pragma region CALLBACKS_IMPLEMENTATION

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if (windowFocus) camera.processMouseMovement(xPos, yPos);
}

void mouse_scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.processMouseScroll(yOffset);
}
#pragma endregion

#pragma region EXT FUNCTIONS IMPLEMENTATION
void fps()
{
    double currentTime = glfwGetTime();
    frameCount++;
    // If a second has passed.
    if (currentTime - previousTime >= 1.0)
    {
        system("cls");
        // Display the frame count here any way you want.
        std::cout << "FPS: " << frameCount << '\n';
        std::cout << "Delta time: " << deltaTime << '\n';
        std::cout << "FOV: " << FOV << '\n';
        std::cout << "Camera position: " << camera.getPosition().x << " " << camera.getPosition().y << " " << camera.getPosition().z << '\n';
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
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        windowFocus = false;
        camera.resetMouseInput();
    }
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
    if(windowFocus)
    {
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cameraSpeed = 10.0f;
        else
            cameraSpeed = 5.0f;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.processKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.processKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.processKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.processKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.processKeyboard(UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            camera.processKeyboard(DOWN, deltaTime);
    }
    #pragma endregion
    #pragma region FOV
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        FOV += 20.0f*deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        FOV -= 20.0f*deltaTime;
    if (FOV > 100.0f) FOV = 100.0f;
    if (FOV < 45.0f) FOV = 45.0f;
    #pragma endregion
    #pragma region MOUSE_BUTTONS
    // Left button
    static int oldState = GLFW_RELEASE;
    int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (newState == GLFW_RELEASE && oldState == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        windowFocus = true;
    }
    oldState = newState;
    #pragma endregion
}   
#pragma endregion
