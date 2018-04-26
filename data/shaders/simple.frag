#version 330

in vec3 v_normal;
in vec3 to_camera;
in vec2 v_uv;
in vec3 v_color;

uniform vec3 color;

out vec4 fragColor;

void main(void)
{

	vec3 final_color = color;

	//final color
	fragColor =  vec4(final_color, 1.0);
}
