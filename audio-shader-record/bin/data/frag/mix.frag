#version 150
out vec4 color;
in vec2 texCoordVarying;
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

void main (void)
{
	vec3 c = texture(tex0, gl_FragCoord.xy ).rgb;
	vec3 c2 = texture(tex1, gl_FragCoord.xy ).rgb;
	vec3 comp = mix(c, c2, 0.5);
	color = vec4(comp, 1.0);
}
