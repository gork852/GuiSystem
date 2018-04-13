#version 420

attribute vec3 vertPos;
attribute vec3 vertNor;
uniform mat4 P;
uniform mat4 MV;
uniform mat4 World;
varying vec3 fragNor;
uniform vec3 vLight;
varying vec3 light;
varying vec3 fragPos;

void main()
{
	gl_Position = P * MV * vec4(vertPos,1);

	fragNor = (MV * vec4(vertNor, 0.0)).xyz;
	light = normalize((World*vec4(vLight,0)).xyz);
}
