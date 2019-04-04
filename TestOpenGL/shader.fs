
#version 330 core

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

in vec3 vertexColor;
in vec2 texCoord;

out vec4 fragColor;

void main() {
    fragColor = mix(texture(ourTexture1, texCoord), texture(ourTexture2, texCoord), 0.2);
}
