/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: README.txt
Purpose: This file gives information for assignment_2.
Language: english
Platform: txt files
Project: s.park_CS300_2
Author: Sejeong Park, s.park, 180002121
Creation date: 11-07-2021
End Header --------------------------------------------------------*/ 

a.
On imgui window,

In "Normal Vector" tap,
	If click the "Vertex Normal", then it shows vertex normal line. 
	If click the "Face Normal", then it shows face normal line.
	If click the "None", then it does not shows any normal lines.

	If click the "Vertex Based Normal", then it draws object based by vertex normal, 
	and If you click the "Face Based Normal", then it draws object based by face normal.

In "Shader" tap,
	You can select shader(Phong Lighting, Phong Shading, Blinn Shading) in shader.
	If you click "Reload Shader", then it can reload all of shaders.

In "Material" tap,
	In "Surface Color", you can change ambient color and emissive of center object.
	In "Texture Projection",
		If you click "Visualize UV", it shows center object with grid texture.
		You can select texture projection mode(None, Cylindrical, Sphere, Cube) in Texture Projection Mode.
		You can select texture projection pipeline(CPU, GPU) in Texture Projection Pipeline.
		You can select Entity(Position, Normal) in Texture Entity.

In "Global Constants" tap,
	You can change attenuation constant, global ambient, fog color.
	And also you can change the fog distance(the first one is near value, the second is far value).

In "Lighting" tap,
	In "Light Orbit", 
		If you click the "Rotation Orbit" button, the light spheres can be rotated or not.
		You can change the speed of the light spheres.
	In "Light Scenarios",
		There are three scenarios,
		In "Scenario 1", There are 8 lights. These lights of type is Point, and all of the color are same.
		In "Scenario 2", There are 7 lights. These lights of type is Spot, and all of the color are different.
		In "Scenario 3", There are 12 lights. These lights of type is different, and all of the color are also different.
	In "Light Source",
		You can change the number of lights (1 ~ 16).
		There are informations of each lights,
			You can change the type of light(Point, Directional, Spot).
			You can change the color of Ambient, Diffuse and Specular.
			There is information of position (x, z) of the light.
			If you choose spot light, You can change the value of inner angle, outer angle, and fall off.

b.
You have to add glm files in ThirdParty file.

c.
I completed all of parts in this assignment.

d.
nothing.

e. 
File path : Common\shaders
	- PhongLightinng.vert, PhongLighting.frag 
		: this shaders are for phong lighting. Lights are calculated in vertex shader.
	- PhongShading.vert, PhongShading.frag
		: this shaders are for phong shading. Lights are calculated in fragment shader.
	- BlinnShading.vert, BlinnShading.frag
		: this shaders are for blinn shading. Lights are calculated in fragment shader. 
		The reflection vector is different with phong lighting and phong shading, 
		It has half vector between light vector and view vector.
	- LightObject.vert, LightObject.frag
		: this shaders are for lights spheres. The light models have only diffuse color.
	- VertexShader.vert, FragmentShader.frag 
		: this shaders are just standard shaders.

File path : Common
	- shader.cpp : 
		//functions
		- LoadShaders : load shader

	- OBJReader.cpp :
		//functions
		- ReadOBJFile : read obj file

	- Mesh.cpp : In this file, set up vertex, indices, vertex normals, vertex normal displays, face, face normals, face normal displays
		//functions
		- CreateSphere : create sphere which is not from obj file.

File path : Assignment_2
	- Light.cpp & Light.h : 
		- In "Light.h", It has structs of LightInfo, and GlboalInfo.
		- In "Light.cpp", It has Get, Set function of lights information and global information.

	- VAOManager.cpp : 
		//functions
		- GenerateVAO() : generate vao
		- CleanUPVAO() : delete vao
		- UseVertexArrayID, UseVertexArrayID, UseFaceVertexArrayID, UseFaceNormalVertexArrayID : bind vao
		- UnUseVertexArrayID :unbind vao
	
	- VBOManager.cpp :
		//functions
		- LoadOBJFile() : load object file
		- GenerateVBO() : generate vbo
		- SetUVBuffers, SetArrayBuffers, SetIndexBuffers, SetLineBuffers, SetFaceVertexBuffers, SetFaceNormalBuffers, SetUniformBuffers, SetTextureBuffers : bind data buffers
		- SetUpBuffers() : set up all buffers
		- UseVertexBuffers, UseLineBuffers, UseFaceVertexBuffers, UseFaceNormalBuffers : bind vbo so can use
		- UnUseVertexBuffers, UnUseLineBuffers, UnUseFaceVertexBuffers, UnUseFaceNormalBuffers : unbind vbo
		- DrawLine, DrawMesh, DrawFaceMesh, DrawFaceNormalLine : draw mesh and lines using opengl function
		- CalculateOrbit() : calculate orbit vertex
		- UseUBOGlobal, UseUBOLight : bind uniform block buffers.
		- UnUseUBO : unbind uniform block buffers.
		- UseTextures : bind all of texture buffers.
		- SetUVBuffers(ObjectTypeEnum type,  Mesh* mesh) : set uv buffers of mesh.

	- Projection.cpp :
		//functions
		- CameraToWorld : matrix for camera to world, I used this function and inverse matrix in shader for "world to camera" matrix

	- ObjectManager.cpp :
		//functions
		- SetTransforms, SetTransformsForRotation : set up uniform of transforms for shader
		- DrawObject, DrawVertexNormalVector, DrawFaceNormalVector, DrawOrbit : check bool type and then draw objects and lines
		- SetUniforms, SetColor, SetLightPosition, SetUniformMatrix, SetUniformVector : set uniforms for shader
		- UseTextures : call UseTextures function of vbo manager.
		- SetGlobalUniform, SetLightsUniform : set uniform block buffers of global and lights.
		- SetInt, SetFloat : set uniforms for shader
		- SetLightInformation, SetGlobalInformation : set light information and set global information for lights.
		- GetLightInformation, GetGlobalInformation : get light information and global information, it returns each of structs.
		- GetLight : get light in vbo manager.
				
	- ImGuiManager.cpp :
		//functions
		- ImGuiRender 
			: render imgui window and all of tap windows.
		- ShowNormalVector 
			: it shows normal vector tap window, which is showing vectors, and drawing models with vertex or face vector.
		- ShowWindowShader
			: it shows shader tap window, which can change shaders and also reload all of shaders.
		- ShowMaterialWindow
			: it shows material tap window, which can change material information.
		- ShowGlobalConstantsWindow
 			: it shows global constants tap window, which can change global constants information.
		- ShowLightingWindow
			: it shows lighting tap window, which can manage and change lights informations.

	- GLApplication.cpp : In this file, create window
	
	- CS300_Project_Scene.cpp : In this file, create current scene

	
File path : ThirdParty
	- GL, GLEW, GLFW, glm, imgui : this is library.

f.
OS: Microsoft Windows 10 Home
GPU: Intel(R) HD Graphics 620
OpenGL: 4.5

g.
40 hours.

h.
nothing.

<Compare CPU and GPU calculation>
The result of render in CPU and GPU is different depending on shaders. 
In CPU, the center model is rendered overlapped such as ears of bunny. But in the GPU, there is no overlapped in the rendering. 
Especially, this diffenerce is best seen in cube modeling in phong shading. In the phong lighting, the results of the both pipeline look same.
I think that GPU is aimed at handling multiple functions at the same time.

