#version 330 core
layout (location = 0) in vec3 positions;
layout (location = 1) in vec3 color;
out vec3 vertexColor;
uniform vec3 positionOffset;
void main()
{
gl_Position = vec4((positions + positionOffset), 1.0);
vertexColor = color;
}