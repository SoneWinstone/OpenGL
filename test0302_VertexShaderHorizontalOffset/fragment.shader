#version 330 core
in vec4 myColor;
out vec4 color;

uniform float greenValue;   // uniform 是全局的 但是必须在使用的地方定义 不然编译报错 可以在OpenGL代码中修改uniform的值

void main() {
    color = vec4(myColor.r, greenValue, myColor.b, myColor.a);
}