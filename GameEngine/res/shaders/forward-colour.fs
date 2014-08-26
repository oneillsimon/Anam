#version 120

varying vec2 texCoord0;
uniform sampler2D diffuse;
uniform vec4 wireFrameColour;

void main()
{
	gl_FragColor = wireFrameColour;
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}