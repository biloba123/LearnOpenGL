#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material material;
uniform vec3 viewPos;
uniform samplerCube skybox;

out vec4 FragColor;

void main() {
    vec3 I = normalize(FragPos - viewPos);
    vec3 R = refract(I, normalize(Normal), 1.0 / 1.33);
    FragColor = texture(skybox, R);
}
