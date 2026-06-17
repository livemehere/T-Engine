#version 410 core

out vec4 FragColor;
in vec4 vColor;
in vec2 vTexCoord;

flat in int vTextureIndex;
uniform sampler2D uTextures[16];

void main()
{
    FragColor = texture(uTextures[vTextureIndex], vTexCoord) * vColor;
}