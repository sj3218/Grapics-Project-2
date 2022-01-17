/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ImGuiManager.cpp
Purpose: This file is source file for managing ImGui, there are functions for rendering ImGui window.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_2
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/

#include "ImGuiManager.hpp"
#define MAX_LIGHT_NUM_SIZE 16

const float DEGREETORADIAN = PI / 180.f;
const float SPEED = 0.01f;

ImGuiManager::ImGuiManager()
{
    reloadButton = false;
    rotationOrbitButton = true;
    orbitSpeed = 0.005f;
    shader_type = 1;

    scenario_1 = false;
    scenario_2 = false;
    scenario_3 = false;

    visualize_UV = false;
    calculate_UV_GPU = false;
    projection_mode = 1;
    entities = 0;

    k_a = glm::vec3(0.f);
    emissive = glm::vec3(0.f);
}

ImGuiManager::ImGuiManager(GLFWwindow* window)
{
    const char* version = "#version 410";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(version);

    reloadButton = false;
    rotationOrbitButton = true;
    orbitSpeed = 0.005f;
    shader_type = 1;

    scenario_1 = false;
    scenario_2 = false;
    scenario_3 = false;

    visualize_UV = false;
    calculate_UV_GPU = false;
    projection_mode = 1;
    entities = 0;

    k_a = glm::vec3(0.f);
    emissive = glm::vec3(0.f);
}

ImGuiManager::~ImGuiManager()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::ImGuiRender(ObjectManager* objManager)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ShowNormalVector(objManager);
    ImGui::Separator();

    ShowWindowShader();
    ImGui::Separator();

    ShowMaterialWindow(objManager);
    ImGui::Separator();

    ShowGlobalConstantsWindow(objManager);
    ImGui::Separator();

    ShowLightingWindow(objManager);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::ShowNormalVector(ObjectManager* objManager)
{
    if (!ImGui::CollapsingHeader("Normal Vector"))
    {
        return;
    }

    static int vectorType = 2;
    static int drawType = 0;

    ImGui::Text("Show normal vector");
    ImGui::RadioButton("Vertex Normal", &vectorType, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Face Normal", &vectorType, 1);
    ImGui::SameLine();
    ImGui::RadioButton("None", &vectorType, 2);

    switch (vectorType)
    {
    case 0:
        objManager->SetDrawVertexNormalVector(true);
        objManager->SetDrawFaceNormalVector(false);
        break;

    case 1:
        objManager->SetDrawVertexNormalVector(false);
        objManager->SetDrawFaceNormalVector(true);
        break;

    case 2:
        objManager->SetDrawVertexNormalVector(false);
        objManager->SetDrawFaceNormalVector(false);
        break;

    default:
        break;
    }

    ImGui::Separator();
    ImGui::Text("Draw the object");
    ImGui::RadioButton("Vertex Based Normal", &drawType, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Face Based Normal", &drawType, 1);

    switch (drawType)
    {
    case 0:
        objManager->SetVersionOfNormalBased(true);
        break;

    case 1:
        objManager->SetVersionOfNormalBased(false);
        break;

    default:
        break;
    }

}

void ImGuiManager::ShowWindowShader()
{
    if (!ImGui::CollapsingHeader("Shader"))
    {
        return;
    }

    const char* shader_types_name[] = { "PhongLighting", "PhongShading", "BlinnShading" };
    static int current_shader = 1;

    ImGui::Combo("shader", &current_shader, shader_types_name, IM_ARRAYSIZE(shader_types_name));
    shader_type = current_shader;

    if (ImGui::Button("Reload Shader"))
    {
        reloadButton = true;
    }
}

void ImGuiManager::ShowMaterialWindow(ObjectManager* objManager)
{
    if (!ImGui::CollapsingHeader("Material"))
    {
        return;
    }

    static glm::vec3 material_ambient = glm::vec3(0.f);
    static glm::vec3 material_emissive = glm::vec3(0.f);
    
    /////////////////material information/////////////////////////
    ImGui::Text("Surface Color");
    ImGui::ColorEdit3("Ambient", (float*)&material_ambient);
    ImGui::ColorEdit3("Emissive", (float*)&material_emissive);
    ImGui::Separator();

    k_a = material_ambient;
    emissive = material_emissive;

    /////////////////texture projection////////////////////////////
    ImGui::Text("Texture Projection");
    
    static bool current_visualize_uv = false;
    ImGui::Checkbox("Visualize UV", &current_visualize_uv);
    visualize_UV = current_visualize_uv;

    const char* projection_mode_text[] = {"None", "Cylindrical", "Spherical", "Cube"};
    static int current_projection_mode = 1;

    ImGui::Combo("Texture Projection Mode", &current_projection_mode, projection_mode_text, IM_ARRAYSIZE(projection_mode_text));

    projection_mode = current_projection_mode;

    const char* projection_pipeline_text[] = { "CPU", "GPU" };
    static int current_projection_pipeline = 0;

    ImGui::Combo("Texture Projection Pipeline", &current_projection_pipeline, projection_pipeline_text, IM_ARRAYSIZE(projection_pipeline_text));

    if (current_projection_pipeline == 0)
    {
        calculate_UV_GPU = false;
    }
    else
    {
        calculate_UV_GPU = true;
    }

    const char* texture_entity_text[] = {"Position", "Normal"};
    static int current_texture_entity = 0;

    ImGui::Combo("Texture Entity", &current_texture_entity, texture_entity_text, IM_ARRAYSIZE(texture_entity_text));

    if (current_texture_entity == 0)
    {
        entities = false;
    }
    else if (current_texture_entity == 1)
    {
        entities = true;
    }

}

void ImGuiManager::ShowGlobalConstantsWindow(ObjectManager* objManager)
{
    if (!ImGui::CollapsingHeader("Global Constants"))
    {
        return;
    }

    ///////////////////global information//////////////////////
    GlobalInfo global_info;

    static glm::vec3 attenuation = glm::vec3(1.f, 0.22f, 0.22f);
    ImGui::DragFloat3("Attenuation Constant", (float*)&attenuation, 0.01f, 0.0f, 1.0f);
    global_info.attenuation = attenuation;

    static float global_ambient[3] = { 0.f,0.f,0.2f };;
    ImGui::ColorEdit3("Global Ambient", global_ambient);
    global_info.globalAmbientColor = glm::vec3(global_ambient[0], global_ambient[1], global_ambient[2]);

    static float fog_color[3] = { 0.5f,0.5f,0.5f };
    ImGui::ColorEdit3("Fog Color", fog_color);
    global_info.fogColor = glm::vec3(fog_color[0], fog_color[1], fog_color[2]);

    static glm::vec2 fog_distance = glm::vec2(0.1f, 20.f);
    ImGui::DragFloat2("Fog Distance", (float*)&fog_distance, 0.1f, 0.1f, 100.f);
    global_info.fogNear = fog_distance.x;
    global_info.fogFar = fog_distance.y;

    objManager->SetGlobalInformation(global_info);
}

void ImGuiManager::ShowLightingWindow(ObjectManager* objManager)
{
    if (!ImGui::CollapsingHeader("Lighting"))
    {
        return;
    }
    ///////////////////////////////////////////////////////// light value ///////////////////////////////////////////////////////////////////////////////////
    static int lights_num = 1;
    const char* light_type_name[3] = { "Point", "Directional", "Spot" };
    static int light_type[MAX_LIGHT_NUM_SIZE] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    static float light_ambient_color[MAX_LIGHT_NUM_SIZE][3] = { 0.2f, 0.2f,0.2f };
    static float light_diffuse_color[MAX_LIGHT_NUM_SIZE][3] = { {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},
                                                                {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f} };
    static float light_specular_color[MAX_LIGHT_NUM_SIZE][3] = { {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},
                                                                {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f} };

    static float inner_angle[MAX_LIGHT_NUM_SIZE] = { 45.f,45.f, 45.f, 45.f, 45.f, 45.f, 45.f, 45.f,45.f,45.f, 45.f, 45.f, 45.f, 45.f, 45.f, 45.f };
    static float outer_angle[MAX_LIGHT_NUM_SIZE] = { 60.f, 60.f, 60.f, 60.f, 60.f, 60.f, 60.f, 60.f, 60.f, 60.f, 60.f, 60.f, 60.f, 60.f, 60.f, 60.f };
    static float fall_off[MAX_LIGHT_NUM_SIZE] = { 5.f, 5.f, 5.f, 5.f, 5.f, 5.f, 5.f, 5.f, 5.f, 5.f, 5.f, 5.f, 5.f, 5.f, 5.f, 5.f };

    //////////////rotation orbit check box//////////////////////
    static bool rotation_orbit_check_box = true;
    ImGui::Text("Light Orbit");
    ImGui::Checkbox("Rotation Orbit", &rotation_orbit_check_box);

    if (rotation_orbit_check_box)
    {
        rotationOrbitButton = true;
    }
    else
    {
        rotationOrbitButton = false;
    }

    static float orbit_speed = 0.5f;
    ImGui::SliderFloat("Speed", &orbit_speed, -1.f, 1.f);
    orbitSpeed = orbit_speed * SPEED;

    //////////////choose light scenarios////////////////////////
    ImGui::Separator();
    ImGui::Text("Light Scenarios");

    if (ImGui::Button("Scenario 1"))
    {
        scenario_1 = true;
        scenario_2 = false;
        scenario_3 = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Scenario 2"))
    {
        scenario_1 = false;
        scenario_2 = true;
        scenario_3 = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Scenario 3"))
    {
        scenario_1 = false;
        scenario_2 = false;
        scenario_3 = true;
    }

    if (scenario_1 == true)
    {
        lights_num = 8;
        
        int type = 0;
        float color[3] = { 1.f,1.f,1.f };
        float innerAngle = 45.f;
        float outerAngle = 60.f;
        float fallOff = 5.f;

        for (int i = 0; i < lights_num; ++i)
        {
            light_type[i] = type;

            light_ambient_color[i][0] = color[0];
            light_ambient_color[i][1] = color[1];
            light_ambient_color[i][2] = color[2];

            light_diffuse_color[i][0] = color[0];
            light_diffuse_color[i][1] = color[1];
            light_diffuse_color[i][2] = color[2];

            light_specular_color[i][0] = color[0];
            light_specular_color[i][1] = color[1];
            light_specular_color[i][2] = color[2];

            inner_angle[i] = innerAngle;
            outer_angle[i] = outerAngle;
            fall_off[i] = fallOff;
        }
        scenario_1 = false;
    }

    if (scenario_2 == true)
    {
        lights_num = 7;

        int type = 2;
        float color[7][3] = { {1.f,0.5f,0.5f}, {1.f,1.f,0.f}, {0.5f,1.f,0.5f}, {0.f,1.f,1.f}, {1.f,0.5f,0.5f}, {0.5f,0.5f,1.f}, {1.f,0.f,1.f}};
        float innerAngle = 45.f;
        float outerAngle = 60.f;
        float fallOff = 5.f;

        for (int i = 0; i < lights_num; ++i)
        {
            light_type[i] = type;

            light_ambient_color[i][0] = color[i][0];
            light_ambient_color[i][1] = color[i][1];
            light_ambient_color[i][2] = color[i][2];

            light_diffuse_color[i][0] = color[i][0];
            light_diffuse_color[i][1] = color[i][1];
            light_diffuse_color[i][2] = color[i][2];

            light_specular_color[i][0] = color[i][0];
            light_specular_color[i][1] = color[i][1];
            light_specular_color[i][2] = color[i][2];

            inner_angle[i] = innerAngle;
            outer_angle[i] = outerAngle;
            fall_off[i] = fallOff;
        }

        scenario_2 = false;
    }

    if (scenario_3 == true)
    {
        lights_num = 12;

        int type[12] = {0,1,2,0,1,2, 0,1,2, 0,1,2};
        float color[12][3] = { {1.f,0.f,0.f}, {1.f,0.f,0.f}, {1.f,1.f,0.f}, {1.f,1.f,0.f}, {0.f, 1.f, 0.f}, {0.f, 1.f, 0.f}, 
                                {0.f, 0.5f, 0.5f}, {0.f, 0.5f, 0.5f}, {0.f, 0.f, 1.f}, {0.f, 0.f, 1.f}, {0.5f, 0.f, 0.5f}, {0.5f, 0.f, 0.5f}};
        float innerAngle = 45.f;
        float outerAngle = 60.f;
        float fallOff = 5.f;

        for (int i = 0; i < lights_num; ++i)
        {
            light_type[i] = type[i];

            light_ambient_color[i][0] = color[i][0];
            light_ambient_color[i][1] = color[i][1];
            light_ambient_color[i][2] = color[i][2];

            light_diffuse_color[i][0] = color[i][0];
            light_diffuse_color[i][1] = color[i][1];
            light_diffuse_color[i][2] = color[i][2];

            light_specular_color[i][0] = color[i][0];
            light_specular_color[i][1] = color[i][1];
            light_specular_color[i][2] = color[i][2];

            inner_angle[i] = innerAngle;
            outer_angle[i] = outerAngle;
            fall_off[i] = fallOff;
        }
        scenario_3 = false;
    }

    ////////////////////light information//////////////////////
    ImGui::Separator();
    ImGui::Text("Light Source");

    ImGui::SliderInt("Lights number", &lights_num, 1, MAX_LIGHT_NUM_SIZE);
    objManager->GetLight()->SetNumberOfLights(lights_num);

    for (int i = 0; i < lights_num; ++i)
    {
        LightInfo info = objManager->GetLight()->GetLightInformation(i);

        ImGui::Text("Light #%d", i+1);

        ImGui::PushID(i);

        ImGui::Combo("Light type", &light_type[i], light_type_name, IM_ARRAYSIZE(light_type_name));
        info.lightType = light_type[i];

        ImGui::ColorEdit3("Ambient", light_ambient_color[i]);
        ImGui::ColorEdit3("Diffuse", light_diffuse_color[i]);
        ImGui::ColorEdit3("Specular", light_specular_color[i]);

        info.ambientColor = glm::vec3(light_ambient_color[i][0], light_ambient_color[i][1], light_ambient_color[i][2]);
        info.diffuseColor = glm::vec3(light_diffuse_color[i][0], light_diffuse_color[i][1], light_diffuse_color[i][2]);
        info.specularColor = glm::vec3(light_specular_color[i][0], light_specular_color[i][1], light_specular_color[i][2]);

        if (light_type[i] == 2) // if the light type is spot, show more information
        {
            ImGui::SliderFloat("Inner Angle", &inner_angle[i], 0.f, 90.f);
            ImGui::SliderFloat("Outer Angle", &outer_angle[i], 0.f, 90.f);
            ImGui::SliderFloat("Falloff", &fall_off[i], 0.f, 10.f);

            info.innerAngle = inner_angle[i] * DEGREETORADIAN;
            info.outerAngle = outer_angle[i] * DEGREETORADIAN;
            info.fallOff = fall_off[i];
        }

        ImGui::Text("x: %f, z: %f", info.lightPosition.x, info.lightPosition.z);

        ImGui::Separator();
        ImGui::PopID();

        objManager->SetLightInformation(i, info);
    }

}

bool ImGuiManager::GetReloadButton()
{
    if (reloadButton == true)
    {
        reloadButton = false;
        return true;
    }

    return false;
}

bool ImGuiManager::GetRotationOrbitButton()
{
    return rotationOrbitButton;
}

float ImGuiManager::GetOrbitSpeed()
{
    return orbitSpeed;
}

int ImGuiManager::GetShaderTypes()
{
    return shader_type;
}

int ImGuiManager::GetProjectionMode()
{
    return projection_mode;
}

bool ImGuiManager::GetCalculateGPU()
{
    return calculate_UV_GPU;
}

bool ImGuiManager::GetVisualizeUV()
{
    return visualize_UV;
}

bool ImGuiManager::GetEntity()
{
    return entities;
}

glm::vec3 ImGuiManager::GetAmbientMaterial()
{
    return k_a;
}

glm::vec3 ImGuiManager::GetEmissiveMaterial()
{
    return emissive;
}
