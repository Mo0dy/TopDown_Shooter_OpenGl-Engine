#version 330 core
layout (location = 0) in vec2 vertex;

out vec3 vertexColor;
uniform mat4 model;

void main()
{
gl_Position = model * vec4(vertex, 0.0f, 1.0f);
vertexColor = vec3(1.0f, 1.0f, 1.0f);
}