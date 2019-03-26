#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;
uniform vec3 fontColor;

void main()
{
	color = vec4(texture(ourTexture, TexCoord).r) * vec4(fontColor, 1.0f);
}