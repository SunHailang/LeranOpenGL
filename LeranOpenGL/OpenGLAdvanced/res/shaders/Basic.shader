#shader vertex
#version 330
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}


#shader fragment
#version 330
layout(location = 0) out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;


void main()
{
	FragColor = texture(ourTexture1, TexCoord) + texture(ourTexture2, TexCoord);// texture(texture_diffuse, TexCoord) + texture(texture_specular, TexCoord) + texture(texture_normal, TexCoord);
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}


