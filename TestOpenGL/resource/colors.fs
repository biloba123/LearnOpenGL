#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    //环境光照
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    //漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(0.0, dot(norm, lightDir));
    vec3 diffuse = diff * lightColor;
    //镜面光照
    float specularStrength  = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(0.0, dot(viewDir, reflectDir)), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = objectColor * (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}
