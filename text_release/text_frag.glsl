#version 420

//in vec2 frag_Pos;
in vec2 frag_texPos;

uniform sampler2D textureId;
uniform vec2 tex_size;
uniform vec3 textColor;

void main(){
	
	
	if(texture2D(textureId,vec2(frag_texPos[0]/tex_size[0],1-frag_texPos[1]/tex_size[1])).a > .5){
		//gl_FragColor.a = texture2D(textureId,vec2(frag_texPos[0]/tex_size[0],1-frag_texPos[1]/tex_size[1])).a;
		//gl_FragColor.rgb = texture2D(textureId,vec2(frag_texPos[0]/tex_size[0],1-frag_texPos[1]/tex_size[1])).rgb;
		gl_FragColor.rgb = textColor;
	}
	else{
		/*gl_FragColor.r = frag_texPos[0]/tex_size[0];
		gl_FragColor.g = frag_texPos[1]/tex_size[1];
		gl_FragColor.b = 0;
		gl_FragColor.a = 1;*/
		discard;
	}
}