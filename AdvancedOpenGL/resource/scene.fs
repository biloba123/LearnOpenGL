#version 330 core
in vec3 Position;
in vec3 Normal;

out vec4 FragColor;

uniform samplerCube skybox;
uniform vec3 viewPos;

void main() {
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - viewPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    vec4 texColor = texture(skybox, R);
    
    if (gl_FrontFacing) {
        FragColor = vec4(vec3(1.0, 1.0, 0.0) * (gl_FragCoord.x / 800), 1.0);
    } else {
        FragColor = vec4(vec3(0.0, 1.0, 1.0) * (gl_FragCoord.x / 800), 1.0);
    }
}
