#version 330 core

out vec4 color;
in vec2 TexCoord;
in vec2 SightCoord;

uniform sampler2D image;
uniform sampler2D sightMap;

uniform vec3 spriteColor;

void main()
{	
	vec4 texColor = texture(image, TexCoord);
	vec4 sightColor = texture(sightMap, SightCoord);
	if (sightColor.x < 1)
		discard;
	if (texColor.a < 0.1)
		discard;
	color = vec4(spriteColor, 1.0f) * texColor;
}