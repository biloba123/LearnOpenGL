#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture1;

const float offset = 1.0 / 300.0;

void main() {
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),
        vec2(0, offset),
        vec2(offset, offset),
        vec2(-offset, 0),
        vec2(0, 0),
        vec2(offset, 0),
        vec2(-offset, -offset),
        vec2(0, -offset),
        vec2(offset, -offset)
    );
    
    float kernel[9] = float[](
        0, 0, 0,
        0, 1, 0,
        0, 0, 0
    );
    
    vec3 samplerTex[9];
    for (int i = 0; i < 9; i++) {
        samplerTex[i] = vec3(texture(texture1, TexCoords.st + offsets[i]));
    }
    
    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++) {
        col += kernel[i] * samplerTex[i];
    }
    
    FragColor = vec4(col, 1.0);
}
