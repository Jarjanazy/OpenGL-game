#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 color;

uniform mat4 modelTransformMatrix;
uniform mat4 projectionMatrix;

out vec3 dacolor;
void main()
{

vec4 v = vec4(aPos, 1.0);
vec4 newPosition = modelTransformMatrix * v;
vec4 projectedPosition = projectionMatrix * newPosition;

gl_Position = projectedPosition;

dacolor = color;

}