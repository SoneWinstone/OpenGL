#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 myColor;

void main() {
    gl_Position = vec4(position, 1.0); // 向量的另一种赋值方式
    // gl_Position = vec4(position.xyz, 1.0);
    myColor = color; // 将myColor设置为从顶点数据中得到的输入颜色
}