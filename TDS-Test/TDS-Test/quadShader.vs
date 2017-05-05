#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec3 color;

out vec3 vertexColor;
uniform mat4 model;

void main()
{
gl_Position = model * vec4(vertex, 0.0f, 1.0f);
vertexColor = color;
}