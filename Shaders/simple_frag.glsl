#version 420

varying vec3 fragNor;
varying vec3 light;
uniform vec4 vColor;
//uniform vec4 fragLight;

void main()
{
	//vec3 normal = normalize(fragNor);
	//vec3 color = 0.5*normal + 0.5;
	gl_FragColor = vColor;
	
   float dotMax = dot(fragNor,light)>0 ? dot(fragNor,light) : 0;
   
	gl_FragColor = vColor*(dotMax*.7+.3);
}
