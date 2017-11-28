#version 330 core
in vec3 myColor;
in vec2 TexCoord;

out vec4 color;
// GLSL有一个供纹理对象使用的内建数据类型，叫做采样器(Sampler)，它以纹理类型作为后缀，比如sampler1D、sampler3D
uniform sampler2D myTexture;

void main() {
    /**
     * 使用GLSL内建的texture函数来采样纹理的颜色，它第一个参数是纹理采样器，第二个参数是对应的纹理坐标。
     * texture函数会使用之前设置的纹理参数对相应的颜色值进行采样。这个片段着色器的输出就是纹理的（插值）纹理坐标上的(过滤后的)颜色。
     */
    color = texture(myTexture, TexCoord)/* myColor */;
}