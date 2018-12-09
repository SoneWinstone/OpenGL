#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;   // 法向量
out vec3 Normal;    // 所有的光照计算都是在片段着色器里进行
out vec3 FragPos;   // 计算世界坐标

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    // 为法向量专门定制的模型矩阵。这个矩阵称之为法线矩阵
    // 法线矩阵被定义为「模型矩阵左上角的逆矩阵的转置矩阵」
    // 把被处理过的矩阵强制转换为3×3矩阵，来保证它失去了位移属性以及能够乘以vec3的法向量
    /*
     * 逆矩阵也是一个开销比较大的运算
     * 最好绘制之前用CPU计算出法线矩阵，然后通过uniform把值传递给着色器
     */
    Normal = mat3(transpose(inverse(model))) * normal;
    FragPos = vec3(model * vec4(position, 1.0));
}
