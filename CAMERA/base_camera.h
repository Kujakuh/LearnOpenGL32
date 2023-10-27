/*
*   BASE_CAMERA.H
*   
*   Free flight camera intended to be used as a starting point for other camera implementations
*
*   Constructor will set default camera values but position but any of them can be modified
*
*   Input processing functions to be called in their respective callback mangers
*/

#ifndef BASE_CAMERA_H
#define BASE_CAMERA_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

#pragma region DEFAULT_CAMERA
enum cameraDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const vec3 FRONT = vec3(0.0f, 0.0f, -1.0f);
const vec3 UP = vec3(0.0f, 1.0f, 0.0f);
const vec3 POSITION = vec3(0.0f, 0.0f, 0.0f);

const float YAW = -90.0f;
const float PITCH = 0.0f;

// Default camera options
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
#pragma endregion "Camera movement options"

class BaseCamera
{
protected:
    
    bool firstMouseInput;   // First mouse input flag
    vec3 position;          // Camera position
    int lastX, lastY;
    vec3 front;             // Camera vector space
    vec3 up;
    vec3 right;
    vec3 worldUp;           // Plane movement, up and down
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
        vec3 newFront;
        newFront.y = sin(radians(this->pitch));
        newFront.x = cos(radians(this->yaw)) * cos(radians(this->pitch));
        newFront.z = sin(radians(this->yaw)) * cos(radians(this->pitch));

        // Normalice the vectors to maintain the same speed in all directions of the camera movement
        this->front = normalize(newFront);
        this->right = normalize(cross(this->front, this->worldUp)); // Re-calc horizontal vector
        this->up = normalize(cross(this->right, this->front));      // Re-calc up vector
    }

public:

    BaseCamera(vec3 position = POSITION, vec3 up = UP, vec3 front = FRONT)    
    {
        this->front = front;
        this->position = position;
        this->worldUp = up;

        this->yaw = YAW;
        this->pitch = PITCH;

        this->camSpeed = SPEED;
        this->mouseDPI = SENSITIVITY;

        this->firstMouseInput = true;

        updateCameraVectors();
    }

    void resetMouseInput() {this->firstMouseInput = true;}

    #pragma region GETTERS
    inline mat4 getViewMatrix() { return lookAt(this->position, (this->position + this->front), this->up); }
    inline vec3 getPosition() { return this->position; }
    inline float getYaw() { return this->yaw; }
    inline float getPitch() { return this->pitch; }
    inline float getCamSpeed() { return this->camSpeed; }
    inline float getMouseDPI() { return this->mouseDPI; }
    #pragma endregion
    #pragma region SETTERS
    inline void setPosition(vec3 position) { this->position = position; }
    inline void setYaw(float yaw) { this->yaw = yaw; }
    inline void setPitch(float pitch) { this->pitch = pitch; }
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