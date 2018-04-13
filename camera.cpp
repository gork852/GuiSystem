#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>
#include <gl/GL.h>
#include <gl/glu.h>
#include <iostream>
#include <string>
   
#include "glmincluder.h"
//#include "Include\glm\glm\glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"

#include "camera.h"

glm::mat4 camera;
int g_width;
int g_height;
GLint h_uProjMatrix;
GLint h_uViewMatrix;
GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;
GLint h_uLightPos, h_uLightColor, h_uSunDir, h_uSunCol;
GLint h_uModelMatrix;

GLint h_aPosition, h_aNormal;

void startPhong(GLuint ShadeProg) {
	h_aPosition = glGetAttribLocation(ShadeProg, "aPosition");
	h_aNormal = glGetAttribLocation(ShadeProg, "aNormal");
	h_uProjMatrix = glGetUniformLocation(ShadeProg, "uProjMatrix");
	h_uViewMatrix = glGetUniformLocation(ShadeProg, "uViewMatrix");
	h_uModelMatrix = glGetUniformLocation(ShadeProg, "uModelMatrix");
	//unneeded cause its no longer a uniform
	//h_uNormMatrix = glGetUniformLocation(ShadeProg, "uNormMatrix");

	h_uLightPos = glGetUniformLocation(ShadeProg, "uLightPos");
	h_uLightColor = glGetUniformLocation(ShadeProg, "uLColor");
	h_uSunDir = glGetUniformLocation(ShadeProg, "sunDir");
	h_uSunCol = glGetUniformLocation(ShadeProg, "sunCol");

	h_uMatAmb = glGetUniformLocation(ShadeProg, "uMat.aColor");
	h_uMatDif = glGetUniformLocation(ShadeProg, "uMat.dColor");
	h_uMatSpec = glGetUniformLocation(ShadeProg, "uMat.sColor");
	h_uMatShine = glGetUniformLocation(ShadeProg, "uMat.shine");
}

int set_g_width(int w) {
	return g_width = w;
}

int set_g_height(int h) {
	return g_height = h;
}

int get_g_width() {
	return g_width;
}

int get_g_height() {
	return g_height;
}

void initCamera(){
	camera = glm::lookAt(glm::vec3(1.0),glm::vec3(1.0),glm::vec3(1.0));
}

void SetProjectionMatrix() {
	glm::mat4 Projection = glm::perspective(80.0f, (float)g_width / g_height, 0.1f, 100.f);
	glUniformMatrix4fv(h_uProjMatrix,1,false, glm::value_ptr(Projection));
}

void SetViewMatrix(glm::mat4 mx) {
	glUniformMatrix4fv(h_uViewMatrix,1,false,glm::value_ptr(mx));
}

void SetModel(glm::mat4 transform) {
	glUniformMatrix4fv(h_uModelMatrix,1,false, glm::value_ptr(transform));
}



void SetMaterial(int i,GLuint phongProg) {

	glUseProgram(phongProg);
	switch (i) {
	case 0:
		glUniform3f(h_uMatAmb, 0.02, 0.02, 0.02);
		glUniform3f(h_uMatDif, 0.8, 0.8, 0.8);
		glUniform3f(h_uMatSpec, 1.0, 1.0, 1.0);
		glUniform1f(h_uMatShine, 200.0);
		break;
	case 1:
		glUniform3f(h_uMatAmb, 0.1, 0.1, 0.1);
		glUniform3f(h_uMatDif, 0.9, 0.9, 0.4);
		glUniform3f(h_uMatSpec, 0.3, 0.3, 0.1);
		glUniform1f(h_uMatShine, 200.0);
		break;
	case 2:
		glUniform3f(h_uMatAmb, 0.01, 0.2, 0.01);
		glUniform3f(h_uMatDif, 0.3, 0.8, 0.3);
		glUniform3f(h_uMatSpec, 0.0, 0.0, 0.00);
		glUniform1f(h_uMatShine, 50.0);
		break;
	case 3:
		glUniform3f(h_uMatAmb, 0.1, 0.1, 0.1);
		glUniform3f(h_uMatDif, 0.3, 0.3, 0.9);
		glUniform3f(h_uMatSpec, 0.3, 0.3, 0.3);
		glUniform1f(h_uMatShine, 100.0);
		break;
	}
}



void SetLight(GLuint phongProg) {
	glUseProgram(phongProg);
	glUniform3f(h_uLightPos, .5, .7, 0);
	glUniform3f(h_uLightColor, .3, .3, .3);
}
void SetSun(GLuint phongProg) {
	glUseProgram(phongProg);
	glUniform3f(h_uSunDir, 1, .1, 0);
	glUniform3f(h_uSunCol, .7, .5, .3);
}