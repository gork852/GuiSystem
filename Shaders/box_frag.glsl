#version 420


in vec4 current_cords;
in vec4 current_color;
in vec4 current_boarder_amt;
in vec4 current_boarder_color;

uniform int options;

uniform int doTexture;
uniform sampler2D textureId;
uniform int Twidth;
uniform int Theight;

float distToEdge(vec4 coords){
	//zero to one find closest to zero or one
	coords.x=abs(coords.x-.5);
	coords.y=abs(coords.y-.5);
	//zero to .5 find closest to .5
	return .5-max(coords.x,coords.y);
}

float distToEdgeX(vec4 coords){
	//zero to one find closest to zero or one
	coords.x=abs(coords.x-.5);
	//zero to .5 find closest to .5
	return .5-coords.x;
}
float distToEdgeY(vec4 coords){
	//zero to one find closest to zero or one
	coords.y=abs(coords.y-.5);
	//zero to .5 find closest to .5
	return .5-coords.y;
}

void main() {
  if(doTexture==1){
	if(texture2D(textureId,vec2(current_cords[0]*Twidth,current_cords[1]*Theight)).a == 0)
		discard;
	else
		gl_FragColor.rgb = texture2D(textureId,vec2(current_cords[0]*Twidth,current_cords[1]*Theight)).rgb;
	
	//gl_FragColor = vec4(doTexture/255.0,current_cords[0],current_cords[1],1);
  
  }
  else{
	  gl_FragColor = current_color;
	  float dist = distToEdge(current_cords);
	  float cordMax = max(current_boarder_amt.x,current_boarder_amt.y);
	  if(dist<=cordMax){
		gl_FragColor = current_boarder_color;
	  }
	  
	  float distX = distToEdgeX(current_cords);
	  float distY = distToEdgeY(current_cords);
	  //gl_FragColor*=.1;
	  
	  if(current_boarder_amt.w>distX && current_boarder_amt.w>distY)
	  {
		vec2 p1 = vec2(current_boarder_amt.w,current_boarder_amt.w);
		vec2 p2 = vec2(current_boarder_amt.w,1-current_boarder_amt.w);
		vec2 p3 = vec2(1-current_boarder_amt.w,current_boarder_amt.w);
		vec2 p4 = vec2(1-current_boarder_amt.w,1-current_boarder_amt.w);
		vec2 cur = vec2(current_cords.x,current_cords.y);
		if(distance(p1,cur)>current_boarder_amt.w && distance(p1,cur)<.5)
			discard;//gl_FragColor=vec4(0,0,1,1);
		if(distance(p2,cur)>current_boarder_amt.w && distance(p2,cur)<.5)
			discard;//gl_FragColor=vec4(0,0,1,1);
		if(distance(p3,cur)>current_boarder_amt.w && distance(p3,cur)<.5)
			discard;//gl_FragColor=vec4(0,0,1,1);
		if(distance(p4,cur)>current_boarder_amt.w && distance(p4,cur)<.5)
			discard;//gl_FragColor=vec4(0,0,1,1);
	  }
  }
}