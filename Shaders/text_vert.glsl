#version 420

in vec2 pos;
in vec2 texPos;

uniform vec2 textPos;
uniform vec2 textScale;

//out vec2 frag_pos;
out vec2 frag_texPos;

void main(){
	vec2 tmp = pos;
	//frag_pos = pos;
	frag_texPos = texPos;
	gl_Position = vec4(tmp.x*textScale.x+textPos.x,tmp.y*textScale.y+textPos.y,0,1);
}
