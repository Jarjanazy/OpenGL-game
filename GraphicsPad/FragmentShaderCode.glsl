#version 430

out vec4 daColor;
in vec3 normalWorld;
in vec3 vertexPositionWorld;
in vec4 vertexColor;

in vec3 lightPositionWorld;
in vec3 eyePositionWorld;
in vec4 ambientLight;
in vec4 dominateColor;
in float lightIntensity;
flat in int shadingType;
flat in int chekpointTile;


float distToLight;

void main()
{
	if(shadingType == 0){
		distToLight = length(lightPositionWorld - vertexPositionWorld);

		// Diffuse
		vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
		float brightness = dot(lightVectorWorld, normalize(normalWorld));
		brightness += lightIntensity;
		vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);

		// Specular
		vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
		vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
		float s = clamp(dot(reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
		s = pow(s, 50);
		s = s * lightIntensity;
		vec4 specularLight = vec4(s, s, s, 1);
		
		if(chekpointTile != 1){
			daColor = (1/distToLight) * ((ambientLight + clamp(diffuseLight, 0, 1) + specularLight) * dominateColor);
		}
		else{
			daColor = dominateColor;
		}
	}

	else if(shadingType == 1){
		daColor = vertexColor;
	}
	
	
	
}