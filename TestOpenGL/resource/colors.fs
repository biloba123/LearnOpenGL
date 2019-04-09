#version 330 core
in vec3 GouraudColor;

out vec4 FragColor;

uniform vec3 objectColor;

void main() {
    vec3 result = objectColor * GouraudColor;
    FragColor = vec4(result, 1.0);
}
