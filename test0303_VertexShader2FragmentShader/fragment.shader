#version 330 core
in vec3 posColor;
out vec4 color;

void main() {
    color = vec4(posColor.r, posColor.g, posColor.b, 1.0F);
}