#version 150
out vec4 color;
in vec2 texCoordVarying;
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

uniform float uMixRatio;
uniform float uThreshold;

float chromaVal(vec3 color, vec3 keyColor, float tolerance, float slope) {
		 float d = abs(length(abs(keyColor - color)));
     float edge0 = tolerance * (1.0 - slope);
     float alpha = smoothstep(edge0, tolerance, d);
     return 1. - alpha;
}

void main (void)
{

	vec2 Uv = gl_FragCoord.xy;

	vec4 texel1 = texture(tex0, Uv );
	vec4 texel2 = texture(tex1, Uv );

	float cVal = chromaVal(texel1.rgb, vec3(0.0,0.0,0.0), uMixRatio, uThreshold);
	vec4 col = mix(texel1, texel2, cVal);

	color = col;
}
