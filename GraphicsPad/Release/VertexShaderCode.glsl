#version 430

in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec3 vertex_Color;
in layout(location=2) vec3 normalModel;


out vec3 normalWorld;
out vec3 vertexPositionWorld;
out vec4 vertexColor;
out vec3 lightPositionWorld;
out vec3 eyePositionWorld;
out vec4 ambientLight;
out vec4 dominateColor;
out float lightIntensity;
flat out int shadingType;
flat out int chekpointTiles;
flat out int chekpointTile; // 1 if it is , 0 if not

float distToLight;
vec3 normal_World;
vec3 vertex_Position_World;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;
uniform vec3 light_Position_World;
uniform vec3 eye_Position_World;
uniform vec4 ambient_Light;
uniform vec4 dominate_Color;
uniform float light_Intensity;
uniform int shading_Type;
uniform int chekpoint_Tile; // 1 if it is , 0 if not

void main()
{
	gl_Position = modelToProjectionMatrix * vertexPositionModel;
	normalWorld = vec3(modelToWorldMatrix * vec4(normalModel, 0));
	vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);

	//// vertex coloring /////
	distToLight = length(light_Position_World - vertexPositionWorld);
	
	// Diffuse
	vec3 lightVectorWorld = normalize(light_Position_World - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(normalWorld));
	brightness += light_Intensity;
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);
	vertexColor = diffuseLight;

	// Specular
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eye_Position_World - vertexPositionWorld);
	float s = clamp(dot(reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
	s = pow(s, 50);
	s = s * light_Intensity;
	vec4 specularLight = vec4(s, s, s, 1);

	if(chekpoint_Tile != 1){
			vertexColor = (1/distToLight) * ((ambient_Light + clamp(diffuseLight, 0, 1) + specularLight) * dominate_Color);
		}
	else{
		vertexColor = dominate_Color;
		}



	lightPositionWorld = light_Position_World;
	eyePositionWorld = eye_Position_World;
	ambientLight = ambient_Light;
	dominateColor = dominate_Color;
	lightIntensity = light_Intensity;
	shadingType = shading_Type;
	chekpointTile = chekpoint_Tile;




}