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
    vec3 R = reflect(I, normalize(Normal));
    vec3 reflect = vec3(texture(skybox, R) * texture(material.texture_specular1, TexCoords));
    vec3 diffuse = texture(material.texture_diffuse1, TexCoords).rgb;
    FragColor = vec4(reflect + diffuse, 1.0);
}
