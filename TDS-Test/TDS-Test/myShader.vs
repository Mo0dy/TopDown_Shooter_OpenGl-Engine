#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoord;
out vec2 SightCoord;

uniform mat4 model;
uniform mat4 sight;

void main()
{
TexCoord = vec2(vertex.z, 1 - vertex.w); // Inverts y coordinate of the texture
gl_Position = model * vec4(vertex.xy, 0.0f, 1.0f);
SightCoord = (sight * vec4(gl_Position.xy, 0.0f, 1.0f)).xy;
}