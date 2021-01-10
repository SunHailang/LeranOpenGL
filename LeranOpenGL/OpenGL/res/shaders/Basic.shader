#shader vertex
#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoord;

//out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * aPos;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}

#shader fragment
#version 330 core
out vec4 FragColor;

//in vec3 ourColor;
in vec2 TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    FragColor = texture(u_Texture, TexCoord);
	//FragColor =  vec4(0.0, 0.0, 0.0, 1.0);
}