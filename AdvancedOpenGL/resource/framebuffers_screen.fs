#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    vec4 texColor = texture(texture1, TexCoords);
    float average = 0.2126 * texColor.r + 0.7152 * texColor.g + 0.0722 * texColor.b;
    FragColor = vec4(vec3(average), 1.0);
}
