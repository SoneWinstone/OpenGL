#version 330 core
in vec3 Normal;
in vec3 FragPos;
out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; // 光源的位置向量
uniform vec3 viewPos; // 摄像机位置

void main() {
    // 环境光照添加到场景里
    // 用光的颜色乘以一个很小的常量环境因子，再乘以物体的颜色，然后将最终结果作为片段的颜色
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // 光的方向向量是光源位置向量与片段位置向量之间的向量差。
    // 能够简单地通过让两个向量相减的方式计算向量差。
    // 我们同样希望确保所有相关向量最后都转换为单位向量，所以我们把法线和最终的方向向量都进行标准化
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // 镜面强度(Specular Intensity)
    float specularStrength = 0.5; // 0.5是中等亮度
    // 计算视线方向向量，和对应的沿着法线轴的反射向量
    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect函数要求第一个向量是从光源指向片段位置的向量 所以对lightDir进行取反操作
    vec3 reflectDir = reflect(-lightDir, norm);
    // 计算向量分量
    // 一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小 这里的32是反光度
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // 对norm和lightDir向量进行点乘，计算光源对当前片段实际的漫发射影响。
    // 结果值再乘以光的颜色，得到漫反射分量。
    // 两个向量之间的角度越大，漫反射分量就会越小：
    float diff = max(dot(norm, lightDir), 0.0); // 两个向量之间的角度大于90度，点乘的结果就会变成负数
    vec3 diffuse = diff * lightColor;

    // 镜面、环境光分量和漫反射分量，把它们相加，然后把结果乘以物体的颜色，来获得片段最后的输出颜色
    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0);
}