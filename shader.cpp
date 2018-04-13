#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>
#include <gl/GL.h>
#include <gl/glu.h>
#include <iostream>
#include <string>
#include <vector>
#include "shader.h"

#define chunkSize 128

int testProgErr(GLuint program) {
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	printf("glerr %d at %d\n", glGetError(), __LINE__);
	if (isLinked == GL_FALSE)
	{
		printf("prog not linked? at %d\n", __LINE__);
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		for (int i = 0; i<infoLog.size(); i++) {
			putchar(infoLog[i]);
		}

		//The program is useless now. So delete it.
		glDeleteProgram(program);

		//Provide the infolog in whatever manner you deem best.
		//Exit with failure.
		return 1;
	}
	return 0;
}
int testShaderErr(GLuint shad) {
	GLint success = 0;
	printf("glerr shader %d at %d\n", glGetError(), __LINE__);
	glGetShaderiv(shad, GL_COMPILE_STATUS, &success);
	printf("glerr shader %d at %d\n", glGetError(), __LINE__);
	if (success == GL_FALSE)
	{
		printf("shaders failed to compile more etails coming\n");
		GLint maxLength = 0;
		printf("glerr shader %d at %d\n", glGetError(), __LINE__);
		glGetShaderiv(shad, GL_INFO_LOG_LENGTH, &maxLength);
		printf("glerr shader %d at %d\n", glGetError(), __LINE__);
		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shad, maxLength, &maxLength, &infoLog[0]);

		for (int i = 0; i<infoLog.size(); i++) {
			putchar(infoLog[i]);
		}

		//The program is useless now. So delete it.
		//glDeleteProgram(program);

		//Provide the infolog in whatever manner you deem best.
		//Exit with failure.
		return 1;
	}
	return 0;
}

void printChunks(char* chnk, int n){
	for(int i=0;i<chunkSize*n;i++){
		putchar(chnk[i]);
		if(chnk[i]==0){
			printf("\\");
		}
	}
}
char* readChunk(FILE* f){
	char* chunk = (char*)calloc(chunkSize,1);
	fread(chunk,1,chunkSize,f);
	return chunk;
}

char* appendChunk(char* source,int chunkNum,char* append){
	char* newChunk = (char*)malloc((chunkNum+1)*chunkSize);
	memcpy(newChunk,				    source, chunkNum*chunkSize);
	memcpy(newChunk+chunkNum*chunkSize, append, chunkSize);
	return newChunk;
}

char* readFileByChunk(FILE* f,int* n){
	char* alchnk = readChunk(f);
	int count = 1;
	while(!feof(f)){
		char* chnk = readChunk(f);
		char* nalchnk = appendChunk(alchnk,count++,chnk);
		free(alchnk);
		free(chnk);
		alchnk = nalchnk;
	}
	*n = count;
	return alchnk;
}

GLuint linkShader(GLuint vert, GLuint frag) {
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, frag);
	glAttachShader(shader_programme, vert);
	glLinkProgram(shader_programme);
	return shader_programme;
}

GLuint compileShader(char* name,GLenum shaderType){
	FILE* f;
	fopen_s(&f,name,"r");

	int count = 0;
	char* Source = readFileByChunk(f,&count);
	//printChunks(Source,count);


	printf("glerr shader %d at %d\n", glGetError(), __LINE__);
	GLuint s = glCreateShader (shaderType);
	printf("glerr shader %d at %d\n", glGetError(), __LINE__);
	glShaderSource (s, 1, (const GLchar**)&Source, NULL);
	glCompileShader (s);

	fclose(f);
	return s;
}

void compileBasicShaders(GLuint* v,GLuint* f){
	*v = compileShader("./Shaders/vert.glsl",GL_VERTEX_SHADER);
	*f = compileShader("./Shaders/frag.glsl",GL_FRAGMENT_SHADER);
}