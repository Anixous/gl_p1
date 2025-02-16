#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

uniform float aspect;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 texCoords;

void main()
{ 
   texCoords = aTex; 
   gl_Position = proj * view * model * vec4(aPos.x, aPos.y * aspect, aPos.z, 1.0);
   
}