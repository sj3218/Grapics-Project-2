/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS300_Project_Scene.cpp
Purpose: This file is source file for creating assignment scene.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_2
Author: Sejeong Park, s.park, 180002121
Creation date: 10-18-2021
End Header --------------------------------------------------------*/
#include <iostream>
#include "CS300_Project_Scene.hpp"
#include "shader.hpp"

const glm::vec3 SALMON(1, 0.453f, 0.453f),
LIGHTPINK(1, 0.68f, 0.906f),
RED(1.f, 0.f, 0.f),
GREEN(0.f, 1.f, 0.f),
BLUE(0.f, 0.f, 1.f),
LIGHTBLUE(0.4824f, 0.6275f, 0.8941f),
WHITE(1.f, 1.f, 1.f),
YELLOW(1.f, 1.f, 0.2f);


CS300_Project_Scene::CS300_Project_Scene(int windowWidth, int windowHeight) : Scene(windowWidth, windowHeight)
{
    InitMembers();

    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;

    glm::vec3 eye(0.f, 2.f, 8.0f);
    glm::vec3 target(0.f, 0.f, -1.f);
    glm::vec3 up(0.f, 1.f, 0.f);
    float fov = 0.5f * PI;
    float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    float near = 0.1f;
    float far = 10.f;

    cam.CameraSetUP(eye, target, up, fov, aspect, near, far);
    camera = CameraToWorld(cam);
    projection = glm::perspective(glm::radians(45.f), aspect, 0.1f, 100.f);

    shader_type = 1; 
    calculate_UV_GPU = false;
    projection_mode = 1;
    entities = 0;
    useUV = false;
}

CS300_Project_Scene::~CS300_Project_Scene()
{
    InitMembers();
    delete imGui;
}

int CS300_Project_Scene::Init()
{
    programID = LoadShaders("../Common/shaders/VertexShader.vert",
        "../Common/shaders/FragmentShader.frag");

    phongLightingShader = LoadShaders("../Common/shaders/PhongLighting.vert", "../Common/shaders/PhongLighting.frag");
    phongShadingShader = LoadShaders("../Common/shaders/PhongShading.vert", "../Common/shaders/PhongShading.frag");
    blinnShadingShader = LoadShaders("../Common/shaders/BlinnShading.vert", "../Common/shaders/BlinnShading.frag");
    lightingShader = LoadShaders("../Common/shaders/LightObject.vert", "../Common/shaders/LightObject.frag");
    SetupBuffers();


    //objectManager.lights.SetNumberOfLights(1);

    return Scene::Init();
}

void CS300_Project_Scene::CleanUp()
{
    glDeleteProgram(programID);
    objectManager.CleanUP();
}

void CS300_Project_Scene::ResizeWindow(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

int CS300_Project_Scene::Render()
{
    camera = CameraToWorld(cam);
    float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    projection = glm::perspective(glm::radians(45.f), aspect, 0.1f, 100.f);

    //glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glUseProgram(programID);
    objectManager.SetShaderProgramID(&programID);
    glm::mat4 orbitMat = glm::mat4(0.50f);
    objectManager.SetUniforms(projection, camera, orbitMat);
    objectManager.DrawOrbit();

    //////////////value//////////////////////////
    float distance = 10.f / PI;
    glm::vec3 scaleVector = glm::vec3(1.f);
    glm::vec3 translate = glm::vec3(0.f);
    glm::vec3 rotateVector = glm::vec3(0.f, 1.0f, 0.f);
    float angleOfRotation = 0.f;
    glm::vec3 lightPosition = glm::vec3(4.f, 2.f, 0.f);

    glm::vec3 centerModelScale = glm::vec3(0.7f);
    glm::vec3 centerModelTranslate = glm::vec3(0.f);

    /////////////////set global uniform//////////
    int numlights = objectManager.GetLight()->GetNumberOfLights();

    GlobalInfo global_info = objectManager.GetGlobalInformation();
    glClearColor(global_info.fogColor.x, global_info.fogColor.y, global_info.fogColor.z, 1.f);
    objectManager.SetGlobalUniform(numlights, global_info.fogFar, global_info.fogNear, global_info.attenuation, global_info.fogColor, global_info.globalAmbientColor);

    float k_d = 0.1f;
    objectManager.SetLightsUniform();
    objectManager.SetFloat("diffuseMaterial", k_d);
    objectManager.SetUniformVector("emissive", glm::vec3(0.0f));
    objectManager.SetUniformVector("cameraPosition", cam.GetEyeVector3());

    /////////////////draw lights////////////////
    glUseProgram(lightingShader);
    objectManager.SetShaderProgramID(&lightingShader);

    scaleVector = glm::vec3(0.05f);
    translate = glm::vec3(distance, 0.f, 0.f);
    rotateVector = glm::vec3(0.f, 1.0f, 0.f);
    angleOfRotation = 0.f;

    for (int i = 0; i < numlights; ++i)
    {
        createdSphere[i] = objectManager.SetTransformsForRotation(ObjectTypeEnum::eCREATE_SPHERE, translate, scaleVector, angleOfRotation + rotationAngle, rotateVector);
        objectManager.GetLight()->SetLightPosition(i, centerModelTranslate, translate, rotateVector, angleOfRotation + rotationAngle);
        objectManager.DrawObject(ObjectTypeEnum::eSPHERE, projection, camera, objectManager.GetLight()->GetLightInformation(i).diffuseColor, false);
        angleOfRotation += (TWOPI / numlights);
    }
    objectManager.SetLightsUniform();

    /////////////////draw bunny////////////////
    shader_type = imGui->GetShaderTypes();
    if (shader_type == 0)
    {
        glUseProgram(phongLightingShader);
        objectManager.SetShaderProgramID(&phongLightingShader);
    }
    else if (shader_type == 1)
    {
        glUseProgram(phongShadingShader);
        objectManager.SetShaderProgramID(&phongShadingShader);
    }
    else
    {
        glUseProgram(blinnShadingShader);
        objectManager.SetShaderProgramID(&blinnShadingShader);
    }

    int current_projection_mode = imGui->GetProjectionMode();
    bool current_entities = imGui->GetEntity();
    if (projection_mode != current_projection_mode || entities != current_entities)
    {
        //none, cylindrical, spherical, cube
        projection_mode = current_projection_mode;
        entities = current_entities;

        int model_mesh_index = 3;
        Mesh* mesh = objectManager.GetMesh(model_mesh_index);

        if (projection_mode == 1)
        {
            mesh->calcUVs(entities, Mesh::UVType::CYLINDRICAL_UV);
        }
        else if (projection_mode == 2)
        {
            mesh->calcUVs(entities, Mesh::UVType::SPHERICAL_UV);
        }
        else if (projection_mode == 3)
        {
            mesh->calcUVs(entities, Mesh::UVType::CUBE_MAPPED_UV);
        }
        objectManager.GetVboManager()->SetUVBuffers(ObjectTypeEnum::eBUNNY, mesh);
    }
    
    useUV = imGui->GetVisualizeUV();
    if (projection_mode == 0)
    {
        objectManager.SetInt("isTexture", false);
    }
    else
    {
        objectManager.UseTextures(useUV);
        objectManager.SetInt("isTexture", true);
    }
    calculate_UV_GPU = imGui->GetCalculateGPU();

    k_a = imGui->GetAmbientMaterial();
    emissive = imGui->GetEmissiveMaterial();
    k_d = 0.5f;
    objectManager.SetInt("entities", imGui->GetEntity());
    objectManager.SetFloat("ambientStrength", k_d);
    objectManager.SetUniformVector("ambientMaterial", k_a);
    objectManager.SetUniformVector("emissive", emissive);
    objectManager.SetUniformVector("cameraPosition", cam.GetEyeVector3());
    objectManager.SetFloat("calculateUV_GPU", calculate_UV_GPU);
    objectManager.SetInt("projectionMode", projection_mode);
    angleOfRotation = 0.f;
    rotateVector = glm::vec3(0.f, 1.0f, 0.f);
    bunny = objectManager.SetTransforms(ObjectTypeEnum::eBUNNY, centerModelTranslate, centerModelScale, angleOfRotation, rotateVector);
    objectManager.DrawObject(ObjectTypeEnum::eBUNNY, projection, camera, RED, true);

    /////////////////draw quad////////////////
    objectManager.SetUniformVector("ambientMaterial", glm::vec3(0.0f));
    objectManager.SetUniformVector("emissive", glm::vec3(0.0f));
    objectManager.SetInt("isTexture", false);
    translate = glm::vec3(0.f, -1.f, -1.f);
    scaleVector = glm::vec3(4.f, 4.f, 4.f);
    angleOfRotation = -PI / 2.f;
    rotateVector = glm::vec3(1.f, 0.f, 0.f);
    cube = objectManager.SetTransforms(ObjectTypeEnum::eQUAD, translate, scaleVector, angleOfRotation, rotateVector);
    objectManager.DrawObject(ObjectTypeEnum::eQUAD, projection, camera, RED, false);


    glEnable(GL_DEPTH_TEST);
    glUseProgram(0);

    imGui->ImGuiRender(&objectManager);
    if (imGui->GetReloadButton() == true)
    {
        programID = ReLoadShaders(&programID, "../Common/shaders/VertexShader.vert", "../Common/shaders/FragmentShader.frag");
        phongLightingShader = ReLoadShaders(&phongLightingShader, "../Common/shaders/PhongLighting.vert", "../Common/shaders/PhongLighting.frag");
        phongShadingShader = ReLoadShaders(&phongShadingShader, "../Common/shaders/PhongShading.vert", "../Common/shaders/PhongShading.frag");
        blinnShadingShader = ReLoadShaders(&blinnShadingShader, "../Common/shaders/BlinnShading.vert", "../Common/shaders/BlinnShading.frag");
        lightingShader = ReLoadShaders(&lightingShader, "../Common/shaders/LightObject.vert", "../Common/shaders/LightObject.frag");
    }
    return 0;
}

int CS300_Project_Scene::postRender()
{
    if (imGui->GetRotationOrbitButton())
    {
        rotationAngle += imGui->GetOrbitSpeed();
    }
    return 0;
}

void CS300_Project_Scene::InitMembers()
{
    programID = 0;
}

void CS300_Project_Scene::SetupNanoGUI(GLFWwindow* window)
{
    imGui = new ImGuiManager(window);
}

void CS300_Project_Scene::SetupBuffers()
{
    objectManager.Init();
    objectManager.SetShaderProgramID(&programID);

    return;
}



