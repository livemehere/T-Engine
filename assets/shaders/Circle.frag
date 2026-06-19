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
    float d = length(vLocalPos.xy);
    float thickness = clamp(vThickness, 0.0, 1.0);
    float fade = clamp(vFade, 0.0001, 1.0);

    float outerAlpha = 1.0 - smoothstep(1.0 - fade, 1.0, d);
    float innerRadius = 1.0 - thickness;
    float innerAlpha = innerRadius <= 0.0 ? 1.0 : smoothstep(innerRadius, innerRadius + fade, d);
    float circle = outerAlpha * innerAlpha;

    if(circle == 0.0) discard;

    FragColor = texture(uTextures[vTextureIndex], vTexCoord) * vColor;
    FragColor.a *= circle;
}
