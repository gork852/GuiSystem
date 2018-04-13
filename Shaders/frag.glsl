#version 420

uniform vec4 inputColour;
in vec4 pos;
out vec4 fragColour;

void main() {
  //fragColour = inputColour;
  
  fragColour = vec4((pos.xyz+vec3(1,1,1))/2,1);
  fragColour *= inputColour;
  //fragColour = vec4(1,0,0,1);
}