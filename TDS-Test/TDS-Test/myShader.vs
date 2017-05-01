#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoord;

void main()
{
TexCoord = vertex.zw;
gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
}