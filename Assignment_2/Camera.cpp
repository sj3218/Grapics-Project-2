/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.cpp
Purpose: This file is source file for Camera class. There are functions for making camera.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_2
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/

#include "Camera.h"

const float PI = 4.0f * atan(1.0f);

Camera::Camera()
{
    eye = glm::vec3(0.f, 0.f, 0.f);

    right = glm::vec3(1.f, 0.f, 0.f);
    up = glm::vec3(0.f, 1.f, 0.f);
    back = glm::vec3(0.f, 0.f, -1.f);

    width = 2.f * static_cast<float>(distance * std::tan(45.f));
    height = width;

    nearDistance = 0.1f;
    farDistance = 10.f;
    distance = nearDistance;
}

void Camera::CameraSetUP(glm::vec3 eyePosition, glm::vec3 lookPosition, glm::vec3 upVector, float fov, float aspect, float near, float far)
{
    eye = eyePosition;
    look = glm::normalize(lookPosition - eyePosition);

    back = -look;
    right = glm::normalize(glm::cross(upVector, back));
    up = upVector;

    distance = near;
    width = 2.f * static_cast<float>(distance * std::tan(fov / 2.0f));
    height = width / aspect;

    nearDistance = near;
    farDistance = far;
}

glm::vec4 Camera::Viewport() const
{
    return glm::vec4(width, height, distance, 0.f);
}

glm::vec4 Camera::GetRightVector() const
{
    return glm::vec4(right, 0.f);
}

glm::vec4 Camera::GetUpVector() const
{
    return glm::vec4(up, 0.f);
}

glm::vec4 Camera::GetBackVector() const
{
    return glm::vec4(back, 0.f);
}

glm::vec4 Camera::GetEyePoint() const
{
    return glm::vec4(eye, 1.f);
}

glm::vec3 Camera::GetEyeVector3() const
{
    return eye;
}

float Camera::GetNearDistance() const
{
    return nearDistance;
}

float Camera::GetFarDistance() const
{
    return farDistance;
}
