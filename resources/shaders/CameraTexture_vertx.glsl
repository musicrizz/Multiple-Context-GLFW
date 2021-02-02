#version 330 core
#pragma optimize(off)
#pragma debug(on)

uniform mat4 projection_mtx;
uniform mat4 view_mtx;
uniform mat4 model_mtx;

in vec4 position;
in vec2 uv_coord;

out vec2 fs_uv_coord;

void main(void)    {
	fs_uv_coord = uv_coord;
	gl_Position = projection_mtx * view_mtx * model_mtx * position ;
	
}
