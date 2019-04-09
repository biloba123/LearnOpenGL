#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 GouraudColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float strength;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec3 normal = mat3(transpose(inverse(view * model))) * aNormal;
    vec3 fragPos = vec3(view * model * vec4(aPos, 1.0));
    
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
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(0.0, dot(viewDir, reflectDir)), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    GouraudColor = ambient + diffuse + specular;
    
}
