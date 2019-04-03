#version 330 core

uniform float offset;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 vertexColor;

void main() {
    gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);
    vertexColor = aColor;
}
