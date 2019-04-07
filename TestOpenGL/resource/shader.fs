#version 330 core
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float mixValue;

void main() {
    fragColor = mix(texture(ourTexture1, texCoord), texture(ourTexture2, texCoord), mixValue);
}
