#version 150
out vec4 color;
in vec2 texCoordVarying;
uniform sampler2DRect tex0;

uniform float uSaturation;

 vec3 changeSaturation(vec3 color, float saturation) {
 	float luma = dot(vec3(0.2125, 0.7154, 0.0721) * color, vec3(1.));
 	return mix(vec3(luma), color, saturation);
 }

void main (void)
{

	vec2 Uv = gl_FragCoord.xy;
	//Uv.x = 1.0 - abs(1.0 - Uv.x * 2.0);
	
	vec3 texel = texture(tex0, Uv ).rgb;
	texel = changeSaturation(texel, uSaturation);

	vec4 col = vec4(texel, 1.0);

	color = col;
}
