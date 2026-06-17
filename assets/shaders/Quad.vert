#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in int aTextureIndex;

out vec4 vColor;
out vec2 vTexCoord;
flat out int vTextureIndex;

uniform mat4 uViewProjection;

void main()
{
    gl_Position = uViewProjection * vec4(aPos, 1.0);
    vColor = aColor;
    vTexCoord = aTexCoord;
    vTextureIndex = aTextureIndex;
}