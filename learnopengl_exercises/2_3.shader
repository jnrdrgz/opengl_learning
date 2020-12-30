#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
out vec4 position_fr;

void main()
{
	gl_Position = position;
	position_fr = gl_Position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec4 position_fr;

void main()
{
	color = position_fr;
};

