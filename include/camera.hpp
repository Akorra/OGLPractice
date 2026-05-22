#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

// defaults
inline constexpr float YAW         = -90.0f;
inline constexpr float PITCH       = 0.0f;
inline constexpr float SPEED       = 2.5f;
inline constexpr float SENSITIVITY = 1.0f;
inline constexpr float ZOOM        = 45.0f;

class Camera 
{
public:
    // camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    glm::vec3 eulerAngles; //< yaw, pitch, roll
 
    // camera options
    float movementSpeed    = SPEED;
    float mouseSensitivity = SENSITIVITY;
    float zoom             = ZOOM; 

    Camera(glm::vec3 pos, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : position(pos), worldUp(up), eulerAngles(glm::vec3(YAW, PITCH, 0.0f)) { updateCameraVectors(); }

    glm::mat4 GetViewMatrix() const { return glm::lookAt(position, position + front, up); }

    void ProcessKeyboard(CameraMovement direction, float deltaTime)
    {
        float velocity = movementSpeed * deltaTime;
        switch (direction)
        {
        case FORWARD:
            position += front * velocity; break;
        case BACKWARD:
            position -= front * velocity; break;
        case LEFT:
            position -= right * velocity; break;
        case RIGHT:
            position += right * velocity; break;
        default: 
            break;
        }

        updateCameraVectors();
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        eulerAngles.x += xoffset*mouseSensitivity;
        eulerAngles.y += yoffset*mouseSensitivity;

        if(constrainPitch)
            eulerAngles.y = glm::clamp(eulerAngles.y, -89.0f, 89.0f);
        
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset) { zoom = glm::clamp(zoom-yoffset, 1.0f, 45.0f); }

private:
    void updateCameraVectors()
    {
        front.x = cos(glm::radians(eulerAngles.x)) * cos(glm::radians(eulerAngles.y));
        front.y = sin(glm::radians(eulerAngles.y));
        front.z = sin(glm::radians(eulerAngles.x)) * cos(glm::radians(eulerAngles.y));

        front = glm::normalize(front);

        right = glm::normalize(glm::cross(front, worldUp)); // cross gives vector perpendicular to both
        up    = glm::normalize(glm::cross(right, front));
    }
};