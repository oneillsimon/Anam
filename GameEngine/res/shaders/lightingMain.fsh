#include "sampling.glh"

float CalcShadowMapAmount(sampler2D shadowMap, vec4 initialShadowMapCoords)
{
	vec3 shadowMapCoords = (initialShadowMapCoords.xyz / initialShadowMapCoords.w) ;
	
	return SampleShadowMap(shadowMap, shadowMapCoords.xy, shadowMapCoords.z);
}

void main()
{
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoord(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementMapScale, displacementMapBias);

	vec3 normal = normalize(tbnMatrix * (255.0 / 128.0 * texture2D(normalMap, texCoords.xy).xyz - 1));
	
	vec4 lightingAmount = CalcLightingEffect(normal, worldPos0) * CalcShadowMapAmount(R_shadowMap, shadowMapCoords0);
	
    gl_FragColor = texture2D(diffuse, texCoords.xy) * lightingAmount;
}