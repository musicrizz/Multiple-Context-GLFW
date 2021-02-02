#version 330 core
#pragma optimize(off)
#pragma debug(on)

uniform ivec2 viewport;

layout (location = 1) in vec4 position;

void main(void) {
	//vec4 pos = projection_matrix * (model_matrix * position);
	gl_PointSize = viewport.y;
	gl_Position = position;
}
