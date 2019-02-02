#pragma once
#include <glm\glm.hpp>

struct MyModel
{
	glm::vec3 lightPosition;
	float lightIntensity = 0.5f;

	// 0 if light source,  1 if bridge,  2 if viechle
	int pickedObject;
	// 0 if per fragment,  1 if per vertex
	int shadingType = 0; // initially 0
};