#include <stdlib.h> 
#include <stdio.h>
#include <math.h>

#include <GL/glew.h>

#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <glfw3.h>

#include <iostream>

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")
#pragma once

class Camera
{
private:
    //Default values
    const float zNEAR = 0.1f;
    const float zFAR = 500.f;
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float FOV = 45.0f;
    glm::vec3 startPosition;
    glm::mat4 view;

protected:
    const float cameraSpeedFactor = 2.0f;
    const float mouseSensitivity = 0.1f;

    // Perspective properties
    float zNear;
    float zFar;
    float FoVy;
    int width;
    int height;
    bool isPerspective;

    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 worldUp;

    // Euler Angles
    float yaw;
    float pitch;

    bool bFirstMouseMove = true;
    float lastX = 0.f, lastY = 0.f;

public:
    Camera(const int width, const int height, const glm::vec3& position);
    void MoveForward(float distance);
    void MoveBackward(float distance);
    void MoveUp(float distance);
    void MoveDown(float distance);
    void MoveLeft(float distance);
    void MoveRight(float distance);
    void ProcessMouseScroll(float yOffset);
    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
    void UpdateViewMatrix();
    void UpdateCameraVectors();
    void Set(const int width, const int height, const glm::vec3& position);
    void Reset();
    void Reshape(int windowWidth, int windowHeight);
    const glm::mat4& GetViewMatrix();
    const glm::mat4 GetProjectionMatrix();
    
};
