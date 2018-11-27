#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 color;

struct Material {
    sampler2D   diffuse;      // 漫反射贴图(其实就是纹理) 替换之前的漫反射颜色
    // 这里移除了环境光材质颜色向量，因为环境光颜色几乎在所有情况下都等于漫反射颜色
    sampler2D   specular;     // 镜面光贴图
    float       shininess;    // 反光度 影响镜面高光的散射/半径
};
struct Light {              // 光照属性(强度)
    vec3 position;          // 聚光光源位置
    vec3 direction;         // 聚光的方向向量
    float cutOff;           // 切光角
    float outerCutOff;      // 外切光角 用于平滑聚光外边缘 模拟出光照逐渐变暗的过程

    vec3 ambient;           // 环境光照通常会设置为一个比较低的强度
    vec3 diffuse;           // 漫反射分量通常设置为光所具有的颜色，通常是一个比较明亮的白色
    vec3 specular;          // 镜面光分量通常会保持为vec3(1.0)，以最大强度发光

    float constant;         // 衰减函数常数项 固定是1
    float linear;           // 衰减函数一次项 参考 http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    float quadratic;        // 衰减函数二次项
};

uniform Light light;
uniform Material material;
uniform vec3 lightColor;
uniform vec3 viewPos;       // 摄像机位置

void main() {
    // 计算向量长度
    float distance = length(light.position - FragPos);
    // 衰减值计算
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
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

    // 将衰减值放到光照计算中，将它分别乘以环境光、漫反射和镜面光颜色
    ambient  *= attenuation; // 可以将环境光分量保持不变，让环境光照不会随着距离减少，但是如果使用多于一个的光源，所有的环境光分量将会开始叠加，所以在这种情况下我们也希望衰减环境光照。
    diffuse  *= attenuation;
    specular *= attenuation;
    // 计算θ值，并将它和切光角ϕ对比，来决定是否在聚光的内部
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    // clamp函数，它把第一个参数约束(Clamp)在了0.0到1.0之间
    // 我们现在有了一个在聚光外是负的，在内圆锥内大于1.0的，在边缘处于两者之间的强度值了。
    // 如果我们正确地约束(Clamp)这个值，在片段着色器中就不再需要if-else了，我们能够使用计算出来的强度值直接乘以光照分量
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // ambient  *= intensity; // 不对环境光做出影响，使其总是有一点光
    diffuse  *= intensity;
    specular *= intensity;

    color = vec4(vec3(ambient + diffuse + specular), 1.0);
    /*
    if(theta > light.cutOff)
    {
      // 执行光照计算
      vec3 result = ambient + diffuse + specular;
      color = vec4(result, 1.0);
    }
    else  // 否则，使用环境光，让场景在聚光之外时不至于完全黑暗
      color = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
    */
}