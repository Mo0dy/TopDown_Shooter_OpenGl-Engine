#version 330 core
layout (location = 0) in vec3 positions;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 vertexColor;
out vec2 TexCoord;

uniform vec3 positionOffset;
void main()
{
gl_Position = vec4((positions + positionOffset), 1.0);
vertexColor = color;
TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
}