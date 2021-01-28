#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;


uniform mat4 projection;
uniform mat4 view;
uniform mate4 model;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
layout(location = 0) out vec4 FragColor;

struct Light
{
	// 聚光灯的位置
	vec3 position;
	// 聚光的方向向量
	vec3 direction;
	// 切光角
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constantValue;
	float linearValue;
	float quadraticValue;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

void main()
{
	// 获取顶点位置到光源位置的向量
	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));

	// 执行光照计算
	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	// diffuse
	vec3 normal = normalize(Normal);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	// soft edges (平滑/软化边缘)
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	// attenuation(衰减)
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constantValue + light.linearValue * distance + light.quadraticValue * pow(distance, 2));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
