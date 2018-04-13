#pragma once
#include "BaseBox.h"
#include "TextTex.h"
class textBox :
	public BaseBox
{
public:
	textBox(GLuint p, float dims[4], glm::vec4 color, GLint textProg, TextTex* font,GLint TextSourceTexture, float textTexWidth, float textTexHeight);
	~textBox();
	void setText(char* tex);
	void renderFun();
	void setTextLineLength(int len);
	float textScaleX, textScaleY;
private:
	int linlen;
	GLint TextSourceTexture;
	GLint textProg;
	TextTex* font;
	char* text;
	float textureW, textureH;
	GLuint textvbo;
	GLuint textvao;
	std::vector<float>* pts;
	glm::vec4 textColor;
};

