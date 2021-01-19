#shader vertex
#version 330
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
    TexCoords = aTexCoord;
}

#shader fragment
#version 330
layout(location = 0) out vec4 FragColor;


struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // 反光度
    float shininess;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material material;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;


void main()
{
    // 环境光
    vec3 ambient = material.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(lightPos - FragPos));

    // 漫反射
    float diffStrength = max(dot(lightDir, norm), 0.0);
    vec3 diff = diffStrength * material.diffuse;

    // 镜面光
    vec3 specDir = normalize(vec3(viewPos - FragPos));
    // reflect 函数要求第一个向量是从光源指向片段位置的向量,第二个参数要求是一个法向量
    vec3 reflectDir = reflect(-lightDir, norm);
    // 计算镜面分量
    float specStrength = pow(max(dot(specDir, reflectDir), 0.0), material.shininess);
    vec3 spec = lightPos * (specStrength * material.specular);

    vec3 result = ambient + diff + spec;
    FragColor = vec4(result, 1.0);
}
