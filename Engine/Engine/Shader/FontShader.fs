#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
    if((texture(ourTexture, TexCoord)).r == 0)
	{
		discard;
	}
	color = vec4((texture(ourTexture, TexCoord)).r);
}