#version 420

struct Material {
  vec3 aColor;
  vec3 dColor;
  vec3 sColor;
  float shine;
};

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec3 uColor;

uniform vec3 uLightPos;
uniform vec3 uLColor;
uniform Material uMat;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

mat4 uNormMatrix;

varying vec3 vNorm;
varying vec3 vColor;
varying vec4 vPosition;

uniform float png;

void main() {
  vec4 light;
  
  //vec4 vPosition;
  //vec3 Refl;
  //vec3 Spec, Diffuse;
  //mat4 transNorm = aNormal*;
  vec3 normnorm = aNormal;
  vec3 refl;

  uNormMatrix = uModelMatrix;
  uNormMatrix[3][3]=0;
  normnorm = (uNormMatrix*vec4(normnorm,0)).xyz;
  /* First model transforms */
  vPosition = uModelMatrix* vec4(aPosition.x, aPosition.y, aPosition.z, 1);
  vPosition = /*uViewMatrix*/ vPosition;
  gl_Position = uProjMatrix*uViewMatrix*vPosition;
  
  
  //light =  normalize((/*uModelMatrix**/uViewMatrix*vec4(uLightPos,1) - vPosition));  //FIX
  //float  di = 1.0/(distance(uLightPos, vPosition));//*distance(uLightPos, vPosition));
  //di = 1;
  //float dotProd = max(dot(normnorm,light),0.0);
  //if(dotProd==0.0)dotProd = 2;
  //refl = -light+2.0*dot(light,normnorm)*normnorm;
  vNorm = normalize(normnorm);
  //vColor = vec3(uColor.r, uColor.g, uColor.b);
  /*if(png>0&&0){
     vColor = vec3(uMat.aColor.r*uLColor.x+di*uLColor.x*uMat.dColor.r*dotProd+pow(max(dot(refl,vec3(0,0,1)),0),uMat.shine)*uMat.sColor.x*uLColor.x, 
                   uMat.aColor.g*uLColor.y+di*uLColor.y*uMat.dColor.g*dotProd+pow(max(dot(refl,vec3(0,0,1)),0),uMat.shine)*uMat.sColor.y*uLColor.y, 
                   uMat.aColor.b*uLColor.z+di*uLColor.z*uMat.dColor.b*dotProd+pow(max(dot(refl,vec3(0,0,1)),0),uMat.shine)*uMat.sColor.z*uLColor.z);
  }
  else{*/
    vColor = uColor;
  //}
}

//refl = -L+2.0*max(transNorm(dot)light,0)*transnorm
