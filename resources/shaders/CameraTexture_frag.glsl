#version 330 core

uniform ivec2 viewport;

uniform sampler2D camera_image;
uniform sampler2D canny_image;

uniform mat4 model_mtx;

uniform int draw_op;
uniform int fase;

in vec2 fs_uv_coord;

out vec4 color;

void faseIdle(vec2 st);
void faseSplit(vec2 st);

void main(void)    {
	vec2 st = gl_FragCoord.xy/viewport;
	
	switch(fase) {
	    case 0: faseIdle(st);break;	    
	    default: faseSplit(st);	    	    
	}
	
}


void faseIdle(vec2 st) {
	//color = texture(camera_image, fs_uv_coord);
	color = vec4( texture(camera_image, fs_uv_coord).rgb,  1.0-smoothstep(0.3,0.35,length(fs_uv_coord - vec2(0.5))) );
	//color = vec4(texture(camera_image, fs_uv_coord).rgb, smoothstep(0.0,1.0, 1.0-length(vec2(fs_uv_coord.x,(1.0-fs_uv_coord.y))  )) ); // no
}

void faseSplit(vec2 st)  {
	if(draw_op == 0)  {
		//color = vec4(texture(camera_image, fs_uv_coord));
		color = vec4( texture(camera_image, fs_uv_coord).rgb,  1.0-smoothstep(0.3,0.35,length(fs_uv_coord - vec2(0.5)))  );
		//color = vec4(texture(camera_image, fs_uv_coord).rgb, smoothstep(0.8,0.0,length(fs_uv_coord - vec2(0.5)))); // no
	}else{
		color = vec4(vec3(0.0, 1.0, 0.0), texture(canny_image, fs_uv_coord).r);
	}
}

