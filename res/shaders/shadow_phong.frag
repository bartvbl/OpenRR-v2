#version 450

layout(location = 0) in vec4 fragment_position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 normal;
layout(location = 3) in vec4 light_frag_position;

out vec4 colour;

layout(location = 3) uniform sampler2D diffuseTexture;
layout(location = 40) uniform sampler2D depthTexture;

layout(location = 5) uniform float texturesEnabled;

layout(location = 12) uniform vec4 camera_position;

layout(location = 15) uniform vec4 light_ambient;
layout(location = 16) uniform vec4 light_diffuse;
layout(location = 17) uniform vec4 light_specular;
layout(location = 18) uniform float light_specular_strength;

layout(location = 24) uniform float attenuation_constant;
layout(location = 25) uniform float attenuation_linear;
layout(location = 26) uniform float attenuation_quadratic;

layout(location = 14) uniform vec4 light_position;

layout(location = 19) uniform vec4 material_ambient;
layout(location = 20) uniform vec4 material_diffuse;
layout(location = 21) uniform vec4 material_specular;
layout(location = 22) uniform vec4 material_emission;
layout(location = 23) uniform float material_shininess;

float shadowTest() {
	vec3 coordinate = light_frag_position.xyz / light_frag_position.w;
	coordinate = coordinate * 0.5 + 0.5;
	float currentDepth = coordinate.z;
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(depthTexture, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float depth = texture(depthTexture, coordinate.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - 0.001 > depth ? 0.0 : 1.0; 
		}
	}
	shadow /= 9.0;
	bool outOfBounds = coordinate.x < 0 || coordinate.x > 1 || coordinate.y < 0 || coordinate.y > 1;
	shadow = outOfBounds ? 1 : shadow;
	return shadow;
}

// This was regrettably lifted from the internet.
void main()
{
	vec3 normalized = normalize(normal.xyz);

	// Ambient
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_ambient.xyz;
		
	// Diffuse 
	vec3 light_direction = normalize(light_position.xyz - fragment_position.xyz);
	float diffuse_factor = max(dot(normalized, light_direction), 0.0);
	vec3 diffuse = diffuse_factor * light_diffuse.xyz;

	// Specular
	vec3 viewDir = normalize(camera_position.xyz - fragment_position.xyz);
	vec3 reflectDir = reflect(-light_direction, normalized);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess) * light_specular_strength;
	vec3 specular = spec * light_specular.xyz;  

	vec3 emission = material_emission.xyz;

	float rayDistance = length(light_position - fragment_position);
	float attenuation = 1.0f / (attenuation_constant + attenuation_linear * rayDistance + attenuation_quadratic * (rayDistance * rayDistance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	float shadow = shadowTest() * 0.75;
	diffuse *= shadow;
	specular *= shadow;

	vec4 textureColour = texture2D(diffuseTexture, texCoord) * vec4(ambient + diffuse + specular, 1.0);

	vec4 colour_amb = (vec4(ambient, 1.0) * material_ambient);
	vec4 colour_dif = (vec4(diffuse, 1.0) * material_diffuse);
	vec4 colour_spe = (vec4(specular, 1.0) * material_specular);
	vec4 colour_emi = material_emission;

	vec4 materialColour = vec4(colour_amb.rgb + colour_dif.rgb + colour_spe.rgb + colour_emi.rgb, min(colour_dif.a, colour_emi.a));

	vec4 white = vec4(1, 1, 1, 1);
	white.rgb *= texture(depthTexture, gl_FragCoord.xy / 1000.0f).rgb * 100.0; 

	colour = (textureColour * texturesEnabled) + (materialColour * (1.0 - texturesEnabled));
}