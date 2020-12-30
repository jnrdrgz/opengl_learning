#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
uniform float horizontal_offset;

void main()
{
	gl_Position = vec4(position.x+ horizontal_offset, position.y, position.z, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
};