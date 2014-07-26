#include "sampling.glh"

float CalcShadowMapAmount(sampler2D shadowMap, vec4 initialShadowMapCoords)
{
	vec3 shadowMapCoords = (initialShadowMapCoords.xyz / initialShadowMapCoords.w);
	
	return SampleVarianceShadowMap(shadowMap, shadowMapCoords.xy, shadowMapCoords.z, R_shadowVarianceMin, R_shadowLightBleedingReduction);
}

void main()
{
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoord(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementMapScale, displacementMapBias);

	vec3 normal = normalize(tbnMatrix * (255.0 / 128.0 * texture2D(normalMap, texCoords).xyz - 1));
	
	vec4 lightingAmount = CalcLightingEffect(normal, worldPos0) * CalcShadowMapAmount(R_shadowMap, shadowMapCoords0);
	
    gl_FragColor = texture2D(diffuse, texCoords) * lightingAmount;
}