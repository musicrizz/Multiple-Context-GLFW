#version 330 core
#pragma optimize(off)
#pragma debug(on)

uniform mat4 mtx_projection;
uniform mat4 mtx_model;

layout (location = 1) in vec2 position;
layout (location = 2) in vec2 uv;

out vec2 uv_coord;

void main(void) {
	uv_coord = uv;
	gl_Position = mtx_projection * mtx_model * vec4(position, 0.0, 1.0);
}
