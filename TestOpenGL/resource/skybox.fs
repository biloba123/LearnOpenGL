#version 330 core
in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube skybox;

void main() {
    vec4 texColor = texture(skybox, TexCoords);
    FragColor = texColor;
}
