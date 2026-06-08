#version 410 core

out vec4 FragColor;
in vec4 vColor;
in vec2 vTexCoord;

in float vTextureIndex;
uniform sampler2D uTextures[16];

void main()
{
    int index = int(vTextureIndex);
    FragColor = texture(uTextures[index], vTexCoord) * vColor;
}