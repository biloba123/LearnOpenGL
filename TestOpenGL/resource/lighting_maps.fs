#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    //vec3 position;
    vec3 direction;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {
    vec3 textColor = vec3(texture(material.diffuse, TexCoords));
    //环境光照
    vec3 ambient = textColor * light.ambient;
    //漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(0.0, dot(norm, lightDir));
    vec3 diffuse = textColor * diff * light.diffuse;
    //镜面光照
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(0.0, dot(viewDir, reflectDir)), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
