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
    
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    
    vec3 textColor = vec3(texture(material.diffuse, TexCoords));
    
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    
    if (theta > light.outerCutOff) {
        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
        
        //环境光照
        vec3 ambient = textColor * light.ambient;
        //漫反射光照
        vec3 norm = normalize(Normal);
        float diff = max(0.0, dot(norm, lightDir));
        vec3 diffuse = textColor * diff * light.diffuse;
        //镜面光照
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(0.0, dot(viewDir, reflectDir)), material.shininess);
        vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;
        
//        ambient *= attenuation;
        diffuse = diffuse * attenuation * intensity;
        specular = specular * attenuation * intensity;
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    } else {
        FragColor = vec4(textColor * light.ambient, 1.0);
    }
}
