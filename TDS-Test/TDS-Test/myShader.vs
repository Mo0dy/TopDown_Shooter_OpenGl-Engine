#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoord;

void main()
{
TexCoord = vec2(vertex.z, 1 - vertex.w); // Inverts y coordinate of the texture
gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
}