#version 330 core

in vec4 fs_color;

out vec4 color;

vec3 GammaEncode(vec3 x) {return pow(x, vec3(1.0 / 2.2));}

void main(void)    {

	if(gl_FragCoord.x == gl_FragCoord.y )  {
		color = vec4(0.0,0.0,1.0,1.0);
	}else{
		color = fs_color;
	}
	
}
