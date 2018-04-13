#pragma once
/*#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>
#include <gl/GL.h>
#include <gl/glu.h>*/
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct texChar {
	int width;
	int x, y, w, h, ox, oy;
} texChar;

class TextTex
{
public:
	TextTex(char * defSrc);
	int texWidth(char*);
	int texHeight(int,char*);
	std::vector<float>* TextToTex(char*);
	std::vector<float>* MakeQuad(float x, float y, float w, float h, float tx, float ty);
	std::vector<float>* TextToTex(int, char*);
	int height, ascender, descender, texH, texW;
	~TextTex();
private:
	texChar chars[255];
	


};

