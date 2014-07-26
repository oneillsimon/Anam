varying vec2 texCoord0;
varying vec3 worldPos0;
varying vec4 shadowMapCoords0;
varying mat3 tbnMatrix;

uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform sampler2D displacementMap;

uniform float displacementMapScale;
uniform float displacementMapBias;

uniform sampler2D R_shadowMap;

#include "lighting.glh"