#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 Position;
    vec3 Normal;
} vs_out;

uniform mat4 models[4];

layout (std140) uniform Matrices {
    mat4 view;
    mat4 projection;
};

void main() {
    mat4 model = models[gl_InstanceID];
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.Position = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
}
