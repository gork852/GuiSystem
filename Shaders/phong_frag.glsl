struct Material {
  vec3 aColor;
  vec3 dColor;
  vec3 sColor;
  float shine;
};

varying vec3 vColor;
varying vec3 vNorm;
varying vec4 vPosition;

uniform vec3 uLightPos;
uniform vec3 uLColor;
uniform Material uMat;

vec3 sunDir;
uniform vec3 sunCol;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

uniform mat4 uNormMatrix;

uniform float togl;
uniform float png;

void main() {
   vec4 light;
   vec3 refl;
   float dotProd;
   float sunProd;
   vec4 norm;
   float di;
  //gl_FragColor = vec4(vNorm.x, vNorm.y, vNorm.z, 1);
  if(png>0&&0==1){
      gl_FragColor = vec4(vColor.x, vColor.y, vColor.z, 1);
  }
  else{
      
  
      //vec4 vPosition;
      //vec3 Spec, Diffuse;
      //mat4 transNorm = aNormal*;
      

      
      /* First model transforms */
     
     
      //light =  normalize((/*uModelMatrix**/uViewMatrix*vec4(uLightPos,1)) - vPosition);  //FIX
      
      light =  normalize(vec4(uLightPos,1) - vPosition);  //FIX
      
      
      di = 1.0/(distance(uLightPos, vPosition)*distance(uLightPos, vPosition));
      //di = 1;
      sunDir = normalize(sunDir);
      //vNorm = normalize(vNorm);
      dotProd = (max(dot(light,vNorm),0.0));
      sunProd = (max(dot(vNorm,sunDir),0.0));
      refl = normalize(-light+2.0*dot(light,vNorm)*vNorm);
      vec3 reflSun = normalize(-sunDir+2.0*dot(sunDir,vNorm)*vNorm);
      vec3 tmp = uViewMatrix*vec4(0,0,1,0);
      //vColor = vec3(uColor.r, uColor.g, uColor.b);
      
      
      
      gl_FragColor = vec4(1*uMat.aColor.r*uLColor.x+di*uLColor.x*uMat.dColor.r*dotProd+pow(max(dot(tmp,refl),0),uMat.shine)*uMat.sColor.x*uLColor.x*1, 
                          1*uMat.aColor.g*uLColor.y+di*uLColor.y*uMat.dColor.g*dotProd+pow(max(dot(tmp,refl),0),uMat.shine)*uMat.sColor.y*uLColor.y*1, 
                          1*uMat.aColor.b*uLColor.z+di*uLColor.z*uMat.dColor.b*dotProd+pow(max(dot(tmp,refl),0),uMat.shine)*uMat.sColor.z*uLColor.z*1,
                          1)*1 +
                     vec4(sunCol.x*uMat.dColor.r*sunProd+pow(max(dot(reflSun,tmp/*vec3(0,0,1)*/),0),uMat.shine)*uMat.sColor.x*sunCol.x*1, 
                          sunCol.y*uMat.dColor.g*sunProd+pow(max(dot(reflSun,tmp/*vec3(0,0,1)*/),0),uMat.shine)*uMat.sColor.y*sunCol.y*1, 
                          sunCol.z*uMat.dColor.b*sunProd+pow(max(dot(reflSun,tmp/*vec3(0,0,1)*/),0),uMat.shine)*uMat.sColor.z*sunCol.z*1,
                          1);
      //gl_FragColor = light;
  }
  
  if(togl>0||0==1){
      gl_FragColor = vec4(vNorm,1);
      //gl_FragColor = light;
      //gl_FragColor = vec4(refl,1);
      gl_FragColor = vec4(sunProd,0,0,1);
      //gl_FragColor = vec4(dotProd*di,0,0,1);
 }
}
