#version 330 core
in vec3 Position;
in vec3 Normal;

out vec4 FragColor;

uniform samplerCube skybox;
uniform vec3 viewPos;

void main() {
    vec3 I = normalize(Position - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    vec4 texColor = texture(skybox, R);
    FragColor = texColor;
}
