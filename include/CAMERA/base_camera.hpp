/*
*   BASE_CAMERA.HPP
*   
*   Free flight camera intended to be used as a starting
*   point for other camera implementations.
*
*   Constructor will set default camera values but
*   position but any of them can be modified
*
*   Input processing functions to be called in their 
*   respective callback mangers
*/

#ifndef BASE_CAMERA_H
#define BASE_CAMERA_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


#pragma region DEFAULT_CAMERA
enum cameraDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

// Default camera values
const glm::vec3 dFRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 dUP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 dPOSITION = glm::vec3(0.0f, 0.0f, 0.0f);

#define dYAW -90.0f
#define dPITCH 0.0f

// Default camera options
#define dSPEED 2.5f
#define dSENSITIVITY 0.1f
#define dZOOM 45.0f
#pragma endregion "Camera movement options"

class BaseCamera
{
protected:
    
    bool firstMouseInput;   // First mouse input flag
    glm::vec3 position;          // Camera position
    float lastX, lastY;
    glm::vec3 front;             // Camera vector space
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;           // Plane movement, up and down
    float yaw;              // Euler Angles
    float pitch;
    float camSpeed;         // Camera options
    float mouseDPI;
    float maxPitch = 80.0f; 
    
    // Recalculate the camera direction vectors
    // when any Euler angle is updated
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 newFront;
        newFront.y = sin(glm::radians(this->pitch));
        newFront.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        newFront.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

        // Normalice the vectors to maintain the same speed in all directions of the camera movement
        this->front = glm::normalize(newFront);
        this->right = glm::normalize(glm::cross(this->front, this->worldUp)); // Re-calc horizontal vector
        this->up = glm::normalize(glm::cross(this->right, this->front));      // Re-calc up vector
    }

public:

    BaseCamera(glm::vec3 position = dPOSITION, glm::vec3 up = dUP, glm::vec3 front = dFRONT)
    {
        this->front = front;
        this->position = position;
        this->worldUp = up;

        this->yaw = dYAW;
        this->pitch = dPITCH;

        this->camSpeed = dSPEED;
        this->mouseDPI = dSENSITIVITY;

        this->firstMouseInput = true;

        updateCameraVectors();
    }

    void resetMouseInput() {this->firstMouseInput = true;}

    #pragma region GETTERS
    //inline glm::mat4 getViewMatrix() { return glm::lookAt(this->position, (this->position + this->front), this->up); }
    inline glm::mat4 getViewMatrix() { return glm::lookAt(this->position, (this->position + this->front), this->up); }
    inline glm::vec3 getPosition() { return this->position; }
    inline float getYaw() { return this->yaw; }
    inline float getPitch() { return this->pitch; }
    inline glm::vec3 getFront() { return this->front; }
    inline float getCamSpeed() { return this->camSpeed; }
    inline float getMouseDPI() { return this->mouseDPI; }
    #pragma endregion
    #pragma region SETTERS
    inline void setPosition(glm::vec3 position) { this->position = position; }
    inline void setPositionX(float x) { this->position.x = x; }
    inline void setPositionY(float y) { this->position.y = y; }
    inline void setPositionZ(float z) { this->position.z = z; }
    inline void setFront(glm::vec3 front) { this->front = front; }
    inline void setYaw(float yaw) { this->yaw = yaw; updateCameraVectors(); }
    inline void setPitch(float pitch) { this->pitch = pitch; updateCameraVectors(); }
    inline void setCamSpeed(float camSpeed) { this->camSpeed = camSpeed; }
    inline void setMouseDPI(float mouseDPI) { this->mouseDPI = mouseDPI; }
    #pragma endregion

    #pragma region INPUT_PROCESSING
    void processKeyboard(cameraDirection direction, float deltaTime)
    {
        float camSpeed = this->camSpeed * deltaTime;

        if (direction == FORWARD)this->position += this->front * camSpeed;
        if (direction == BACKWARD) this->position -= this->front * camSpeed;
        if (direction == LEFT) this->position -= this->right * camSpeed;
        if (direction == RIGHT) this->position += this->right * camSpeed;
        if (direction == UP) this->position.y += this->up.y * camSpeed;
        if (direction == DOWN) this->position.y -= this->up.y * camSpeed;
    }

    void processMouseMovement(float xPos, float yPos, bool lockPitch = true)
    {
        if (firstMouseInput)
        {
            lastX = xPos;
            lastY = yPos;
            firstMouseInput = false;
        }

        // Difference between last and current frame mouse position
        float xOffset = xPos -lastX;
        float yOffset = lastY - yPos;
        lastX = xPos;
        lastY = yPos;

        xOffset *= this->mouseDPI;
        yOffset *= this->mouseDPI;

        this->yaw += xOffset;
        this->pitch += yOffset;

        // Avoid camera flip when looking up or down if desired
        if (lockPitch)
        {
            if (this->pitch > maxPitch) this->pitch = maxPitch;
            if (this->pitch < -maxPitch) this->pitch = -maxPitch;
        }

        updateCameraVectors();
    }

    void processMouseScroll(float yOffset)
    {
        /*
        this->zoom -= (float)yOffset;
        if (this->zoom < 1.0f) this->zoom = 1.0f;
        if (this->zoom > 45.0f) this->zoom = 45.0f;
        */
    }
    #pragma endregion "Keyboard and mouse movement a scroll input processing"

};

#endif // !BASE_CAMERA_H