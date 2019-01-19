#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;

uniform float opacity;

void main()
{
    color = texture(ourTexture1, TexCoord) * opacity;
}