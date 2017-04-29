#version 330 core
out vec4 color;
in vec3 vertexColor;
in vec2 outTexCoord;

uniform sampler2D outTexture;

void main()
{
color = texture(outTexture, outTexCoord);
}