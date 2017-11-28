#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 myColor;
uniform float HorizontalOffset;

void main() {
    // 加上水平偏移
    gl_Position = vec4(position.x + HorizontalOffset, position.yz, 1.0);
    myColor = color;
}