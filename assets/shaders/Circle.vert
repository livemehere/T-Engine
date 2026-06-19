#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aLocalPos;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in int aTextureIndex;
layout (location = 5) in float aThickness;
layout (location = 6) in float aFade;

out vec3 vLocalPos;
out vec4 vColor;
out vec2 vTexCoord;
flat out int vTextureIndex;
out float vThickness;
out float vFade;

uniform mat4 uViewProjection;

void main()
{
    gl_Position = uViewProjection * vec4(aPos, 1.0);
    vColor = aColor;
    vTexCoord = aTexCoord;
    vTextureIndex = aTextureIndex;
    vLocalPos = aLocalPos;
    vThickness = aThickness;
    vFade = aFade;
}