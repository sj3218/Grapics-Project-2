/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ImGuiManager.hpp
Purpose: This file is header file for managing ImGui, there are functions for rendering ImGui window.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_2
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/


#pragma once

//Include ImGui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ObjectManager.h"
#include <glm/glm.hpp>

class ImGuiManager
{
public:
    ImGuiManager();
    ImGuiManager(GLFWwindow* window);
    ~ImGuiManager();

    void ImGuiRender(ObjectManager* objManager);

    void ShowNormalVector(ObjectManager* objManager);
    void ShowWindowShader();
    void ShowMaterialWindow(ObjectManager* objManager);
    void ShowGlobalConstantsWindow(ObjectManager* objManager);
    void ShowLightingWindow(ObjectManager* objManager);


    bool GetReloadButton();
    bool GetRotationOrbitButton();
    float GetOrbitSpeed();
    int GetShaderTypes();
    int GetProjectionMode();
    bool GetCalculateGPU();
    bool GetVisualizeUV();
    bool GetEntity();

    glm::vec3 GetAmbientMaterial();
    glm::vec3 GetEmissiveMaterial();

private:
    bool reloadButton;
    bool rotationOrbitButton;
    float orbitSpeed;
    int shader_type;

    bool scenario_1;
    bool scenario_2;
    bool scenario_3;

    bool visualize_UV;
    bool calculate_UV_GPU;
    int projection_mode;
    bool entities;

    glm::vec3 k_a;
    glm::vec3 emissive;
};