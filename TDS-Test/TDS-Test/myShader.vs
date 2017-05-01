#version 330 core
layout (location = 0) in vec2 positions;
layout (location = 1) in vec3 color;

out vec3 vertexColor;

void main()
{
gl_Position = vec4(positions, 0.0f, 1.0f);
vertexColor = color;
}