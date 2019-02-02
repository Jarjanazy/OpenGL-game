#version 330 core
out vec4 FragColor;
in vec3 dacolor;
void main()
{
   FragColor = vec4(dacolor.x, dacolor.y, dacolor.z ,1);
}