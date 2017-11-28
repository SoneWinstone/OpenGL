#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec3 posColor;
uniform float HorizontalOffset;
uniform float VerticalOffset;

void main() {
    gl_Position = vec4(position.x + HorizontalOffset, position.y + VerticalOffset, position.z, 1.0);
    posColor = vec3(position.x + HorizontalOffset, position.y + VerticalOffset, position.z);
}