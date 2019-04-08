#version 330 core
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    //环境光照
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    //漫反射光照
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(0.0, dot(norm, lightDir));
    vec3 diffuse = diff * lightColor;
    //镜面光照
    float specularStrength  = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(0.0, dot(viewDir, reflectDir)), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = objectColor * (ambient + diffuse + specular);
    fragColor = vec4(result, 1.0);
}
