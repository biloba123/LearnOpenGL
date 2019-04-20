#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aOffset;

out VS_OUT {
    vec3 Position;
    vec3 Normal;
} vs_out;

uniform mat4 model;

layout (std140) uniform Matrices {
    mat4 view;
    mat4 projection;
};

void main() {
    vec3 pos = aPos * (gl_InstanceID + 1) / 9 + aOffset;
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vs_out.Position = vec3(model * vec4(pos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
}
