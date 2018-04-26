#version 330

in vec3 v_normal;
in vec3 to_camera;
in vec2 v_uv;

uniform vec3 u_light;
uniform sampler2D u_texture_diffuse;

out vec4 fragColor;

void main(void)
{
	vec3 N = normalize(v_normal);
	vec3 L = normalize(u_light);
	vec3 E = normalize(to_camera);

	vec3 final_color = vec3(0.0, 0.0, 0.0);

	vec3 ambient = vec3(0.1, 0.1, 0.1);

	//use texture color
	vec3 diffuse = texture2D(u_texture_diffuse, v_uv).xyz;

	final_color = diffuse;

	//final color
	fragColor =  vec4(final_color, 1.0);
}
