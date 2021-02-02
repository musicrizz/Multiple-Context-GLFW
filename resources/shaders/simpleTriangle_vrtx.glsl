#version 330 core
#pragma optimize(off)
#pragma debug(on)

in vec4 position;
in vec4 color;

out vec4 fs_color;

void main(void)    {
	fs_color = color;
	gl_Position = position ;
	
}
