#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Texture Coordinates
layout (location = 1) in vec2 aTex;

out vec2 texCoords;

uniform mat4 camMatrix;


void main()
{
	gl_Position = camMatrix * vec4(aPos, 1.0);
	texCoords = aTex;
}