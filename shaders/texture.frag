#version 330

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main(void)
{
    gl_FragColor = texture(ourTexture, TexCoord);
}
