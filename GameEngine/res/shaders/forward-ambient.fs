#version 120

#include "sampling.glh"

varying vec2 texCoord0;
varying vec3 worldPos0;
varying mat3 tbnMatrix;

uniform vec3 R_ambient;
uniform vec3 C_eyePos;
uniform sampler2D diffuse;
uniform sampler2D displacementMap;
uniform float displacementMapScale;
uniform float displacementMapBias;

void main()
{
    vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoord(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementMapScale, displacementMapBias);
    gl_FragColor = texture2D(diffuse, texCoord0.xy) * vec4(R_ambient, 1);
}
