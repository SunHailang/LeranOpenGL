#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mate4 model;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
layout(location = 0) out vec4 FragColor;

/*
	
*/
struct Light 
{
	// 点光源的位置
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	// 常数项
	float constantValue;
	// 一次项
	float linearValue;
	// 二次项
	float quadraticValue;
};

in vec3 FragPos;

uniform Light light;

void main()
{
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constantValue + light.linearValue * distance + light.quadraticValue * pow(distance, 2));
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	// 计算 环境光、漫反射、镜面光颜色
	// ...
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 resultColor = ambient + diffuse + specular;

	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
