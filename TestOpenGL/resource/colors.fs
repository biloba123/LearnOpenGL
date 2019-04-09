#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    vec3 position;
};

uniform Material material;
uniform Light light;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main() {
    //环境光照
    vec3 ambient = material.ambient * light.ambient;
    //漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(0.0, dot(norm, lightDir));
    vec3 diffuse = material.diffuse * diff * light.diffuse;
    //镜面光照
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(0.0, dot(viewDir, reflectDir)), material.shininess);
    vec3 specular = material.specular * spec * light.specular;
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
