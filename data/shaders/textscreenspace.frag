#version 330

in vec3 v_normal;
in vec3 to_camera;
in vec2 v_uv;

uniform vec3 u_light;
uniform sampler2D u_texture_diffuse;

out vec4 fragColor;

void main(void)
{
	//final color
	fragColor =  texture2D(u_texture_diffuse, v_uv);
}
