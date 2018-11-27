#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 color;

struct Material {
    sampler2D   diffuse;      // 漫反射贴图(其实就是纹理) 替换之前的漫反射颜色
    // 这里移除了环境光材质颜色向量，因为环境光颜色几乎在所有情况下都等于漫反射颜色
    sampler2D   specular;     // 镜面光贴图
    sampler2D   emission;     // 放射光贴图
    float       shininess;    // 反光度 影响镜面高光的散射/半径
};
struct Light {              // 光照属性(强度)
    vec3 position;          // 光源位置

    vec3 ambient;           // 环境光照通常会设置为一个比较低的强度
    vec3 diffuse;           // 漫反射分量通常设置为光所具有的颜色，通常是一个比较明亮的白色
    vec3 specular;          // 镜面光分量通常会保持为vec3(1.0)，以最大强度发光
};

uniform Light light;
uniform Material material;
uniform vec3 lightColor;
uniform vec3 viewPos;       // 摄像机位置

void main() {
    // 环境光照添加到场景里
    // 用光的颜色乘以一个很小的常量环境因子，再乘以物体的颜色，然后将最终结果作为片段的颜色
    // 将环境光得材质颜色设置为漫反射材质颜色同样的值
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 光的方向向量是光源位置向量与片段位置向量之间的向量差。
    // 能够简单地通过让两个向量相减的方式计算向量差。
    // 我们同样希望确保所有相关向量最后都转换为单位向量，所以我们把法线和最终的方向向量都进行标准化
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    // 对norm和lightDir向量进行点乘，计算光源对当前片段实际的漫发射影响。
    // 结果值再乘以光的颜色，得到漫反射分量。
    float diff = max(dot(norm, lightDir), 0.0); // 两个向量之间的角度大于90度，点乘的结果就会变成负数
    // 从纹理中采样片段的漫反射颜色值
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    // 镜面强度(Specular Intensity)
    float specularStrength = 0.5; // 0.5是中等亮度
    // 计算视线方向向量，和对应的沿着法线轴的反射向量
    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect函数要求第一个向量是从光源指向片段位置的向量 所以对lightDir进行取反操作
    vec3 reflectDir = reflect(-lightDir, norm);
    // 计算向量分量
    // 一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 采样镜面光贴图，来获取片段所对应的镜面光强度
    // 镜面高光的强度可以通过图像每个像素的亮度来获取。
    // 镜面光贴图上的每个像素都可以由一个颜色向量来表示，比如说黑色代表颜色向量vec3(0.0)，灰色代表颜色向量vec3(0.5)。
    // 取样对应的颜色值并将它乘以光源的镜面强度。一个像素越「白」，乘积就会越大，物体的镜面光分量就会越亮。
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;

    // emission 放射光贴图 直接获取纹理的RGB 不需要考虑环境光照、漫反射和镜面光照的影响
    vec3 emission = texture(material.emission, TexCoords).rgb;

    // 将四个分量相加获得最终的效果
    vec3 result = ambient + diffuse + specular + emission;
    color = vec4(result, 1.0);
}