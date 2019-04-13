#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

//聚光源
struct SpotLight {
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
uniform SpotLight spotLight;
uniform vec3 viewPos;

out vec4 FragColor;

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 viewDir = normalize(viewPos - FragPos);
    FragColor = vec4(CalcSpotLight(spotLight, Normal, FragPos, viewDir), 1.0);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, -light.direction);
    
    //漫反射着色
    float diff = max(0.0, dot(lightDir, normal));
    //镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(0.0, dot(viewDir, reflectDir)), material.shininess);
    //衰减
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    //平滑边缘
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //合并
    vec3 textColor = vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 ambient = light.ambient * textColor;
    vec3 diffuse = light.diffuse * diff * textColor;
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
//    ambient *= attenuation * intensity;
//    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return (ambient + diffuse + specular);
}

