#version 330 core
in vec4 myColor;
out vec4 color;

// uniform 是全局的 但是必须在使用的地方定义 不然编译报错 可以在OpenGL代码中修改uniform的值
// 如果定义了uniform 却没有使用 编译的时候会去掉这个变量 引起错误
uniform float greenValue;

void main() {
    color = vec4(myColor.r, greenValue, myColor.b, myColor.a);
}