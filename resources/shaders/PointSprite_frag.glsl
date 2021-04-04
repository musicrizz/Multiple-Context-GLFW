#version 330 core

uniform sampler2D sprite_texture;
uniform vec2 repeat = vec2(1, 3);

//out vec4 out_color;
//
//void main(void) {
//	out_color = texture(sprite_texture, gl_PointCoord);
//	//out_color = texture2D(sprite_texture, vec2(mod(gl_PointCoord.x * repeat.x, 1), mod(gl_PointCoord.y * repeat.y, 1)));
//	
//	/*
//	out_color = texture(sprite_texture, gl_PointCoord);
//	float g = dot(out_color.rgb, vec3(0.299, 0.587, 0.114));
//	out_color.rgb = vec3(g, g, g);
//	*/
//}

//Glow

uniform float glow_size = 2.5;
uniform vec3 glow_colour = vec3(1.0, 1.0, 1.0);
uniform float glow_intensity = 1;
uniform float glow_threshold = .5;

out vec4 pixel;

void main() {
    pixel = texture(sprite_texture, gl_PointCoord);
    
    if (pixel.a <= glow_threshold) {
        ivec2 size = textureSize(sprite_texture, 0);
	
        float uv_x = gl_PointCoord.x * size.x;
        float uv_y = gl_PointCoord.y * size.y;

        //***************
        //this part is no my code, but I don't remember the original author ..
        float sum = 0.0;
        for (int n = 0; n < 9; ++n) {
            uv_y = (gl_PointCoord.y * size.y) + (glow_size * float(n - 4.5));
            float h_sum = 0.0;
            h_sum += texelFetch(sprite_texture, ivec2(uv_x - (4.0 * glow_size), uv_y), 0).a;
            h_sum += texelFetch(sprite_texture, ivec2(uv_x - (3.0 * glow_size), uv_y), 0).a;
            h_sum += texelFetch(sprite_texture, ivec2(uv_x - (2.0 * glow_size), uv_y), 0).a;
            h_sum += texelFetch(sprite_texture, ivec2(uv_x - glow_size, uv_y), 0).a;
            h_sum += texelFetch(sprite_texture, ivec2(uv_x, uv_y), 0).a;
            h_sum += texelFetch(sprite_texture, ivec2(uv_x + glow_size, uv_y), 0).a;
            h_sum += texelFetch(sprite_texture, ivec2(uv_x + (2.0 * glow_size), uv_y), 0).a;
            h_sum += texelFetch(sprite_texture, ivec2(uv_x + (3.0 * glow_size), uv_y), 0).a;
            h_sum += texelFetch(sprite_texture, ivec2(uv_x + (4.0 * glow_size), uv_y), 0).a;
            sum += h_sum / 9.0;
        }

        pixel = vec4(glow_colour, (sum / 9.0) * glow_intensity);
    }
}

