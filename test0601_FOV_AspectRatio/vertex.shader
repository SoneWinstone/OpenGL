#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec4 myColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // 矩阵乘法顺序是从右往左的
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoord = texCoord;
}
