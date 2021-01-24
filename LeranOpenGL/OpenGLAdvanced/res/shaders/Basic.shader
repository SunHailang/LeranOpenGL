#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}


#shader fragment
#version 330 core
layout(location = 0) out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform vec3 viewPos;

void main()
{
	vec3 ambient = 0.1 * lightAmbient * texture(texture_diffuse, TexCoord).rgb;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(vec3(lightPos - FragPos));

	// 漫反射
	float diffStrength = max(dot(lightDir, norm), 0.0);
	vec3 diff = (diffStrength * lightDiffuse) * texture(texture_diffuse, TexCoord).rgb;

	// 镜面光
	vec3 specDir = normalize(vec3(viewPos - FragPos));
	// reflect 函数要求第一个向量是从光源指向片段位置的向量,第二个参数要求是一个法向量
	vec3 reflectDir = reflect(-lightDir, norm);
	float specStrength = pow(max(dot(specDir, reflectDir), 0.0), 32);
	vec3 spec = (lightColor * (specStrength * lightSpecular)) * texture(texture_specular, TexCoord).rgb;

	vec4 texColor = texture(texture_normal, TexCoord) + texture(texture_height, TexCoord);

	vec4 result = /*texColor + */vec4(ambient, 1.0) +vec4(diff, 1.0) + vec4(spec, 1.0);

	FragColor = result;
}


