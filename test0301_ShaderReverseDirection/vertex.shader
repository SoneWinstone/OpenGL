#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 myColor;

void main() {
    // 让方向上下颠倒
    gl_Position = vec4(position.x, -position.y, position.z, 1.0);
    myColor = color;
}