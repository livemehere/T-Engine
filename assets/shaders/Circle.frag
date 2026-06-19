#version 410 core

out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoord;
in vec3 vLocalPos;
in float vThickness;
in float vFade;

flat in int vTextureIndex;
uniform sampler2D uTextures[16];

void main()
{
    float d = 1.0 - length(vLocalPos.xy);
    float circle = smoothstep(0.0, vFade, d);
    circle *= smoothstep(vThickness + vFade, vThickness, d);

    if(circle == 0.0) discard;

    FragColor = texture(uTextures[vTextureIndex], vTexCoord) * vColor;
    FragColor.a *= circle;
}
