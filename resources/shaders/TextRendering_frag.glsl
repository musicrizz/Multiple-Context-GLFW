#version 330 core

uniform sampler2D char_texture;
uniform vec4 color;

in vec2 uv_coord;

out vec4 out_color;

void main(void) {

	out_color= vec4(color.rgb, texture(char_texture, uv_coord).r * color.a);
    
}


