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
	// 使用平行光只需要一个光的方向 光源指向顶点
	vec3 direction; 
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


uniform Light light;

void main()
{
	// 获取顶点指向光源的方向
	vec3 lightDir = normalize(-light.direction);

	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
