#include "textBox.h"


void callaround(void* point) {
	((textBox*)point)->renderFun();
}

textBox::textBox(GLuint p, float dims[4], glm::vec4 color,GLint textProg,TextTex* font,GLint TextSourceTexture,float textTexWidth, float textTexHeight)
	:BaseBox(p,dims,color)
{
	this->font = font;
	this->textProg = textProg;
	this->textColor = color;
	char* testText = "";
	this->TextSourceTexture = TextSourceTexture;

	createAlternateRenderTarget(1280, 720);
	this->setAlternateRenderCallback(callaround, this);

	pts = font->TextToTex(300, testText);
	glGenVertexArrays(1, &textvao);
	glGenBuffers(1, &textvbo);
	glBindBuffer(GL_ARRAY_BUFFER, textvbo);
	float* points = pts->data();
	glBufferData(GL_ARRAY_BUFFER, pts->size() * sizeof(float), points, GL_STATIC_DRAW);
	glBindVertexArray(textvao);

	glEnableVertexAttribArray(glGetAttribLocation(textProg, "pos"));
	glBindBuffer(GL_ARRAY_BUFFER, textvbo);
	glVertexAttribPointer(glGetAttribLocation(textProg, "pos"),
		2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);



	glEnableVertexAttribArray(glGetAttribLocation(textProg, "texPos"));
	glBindBuffer(GL_ARRAY_BUFFER, textvbo);
	glVertexAttribPointer(glGetAttribLocation(textProg, "texPos"),
		2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (char*)(sizeof(float) * 2));

	textScaleX = .2 / font->height; //use scale of a single character or you get thin text
	textScaleY = .2 / 43;
	textureW = textTexWidth;
	textureH = textTexHeight;
}

textBox::~textBox()
{
}

void textBox::setText(char * tex)
{
	this->font = font;
	this->textProg = textProg;

	char* testText = tex;
	pts = linlen?font->TextToTex(300, testText): font->TextToTex(testText);
	glGenVertexArrays(1, &textvao);
	glGenBuffers(1, &textvbo);
	glBindBuffer(GL_ARRAY_BUFFER, textvbo);
	float* points = pts->data();
	glBufferData(GL_ARRAY_BUFFER, pts->size() * sizeof(float), points, GL_STATIC_DRAW);
	glBindVertexArray(textvao);

	glEnableVertexAttribArray(glGetAttribLocation(textProg, "pos"));
	glBindBuffer(GL_ARRAY_BUFFER, textvbo);
	glVertexAttribPointer(glGetAttribLocation(textProg, "pos"),
		2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);



	glEnableVertexAttribArray(glGetAttribLocation(textProg, "texPos"));
	glBindBuffer(GL_ARRAY_BUFFER, textvbo);
	glVertexAttribPointer(glGetAttribLocation(textProg, "texPos"),
		2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (char*)(sizeof(float) * 2));

	textScaleX = .2 / font->height; //use scale of a single character or you get thin text
	textScaleY = .2 / 43;
}

void textBox::renderFun() {
	
	glUseProgram(textProg);

	glBindTexture(GL_TEXTURE_2D, TextSourceTexture);

	glUniform2f(glGetUniformLocation(textProg, "textPos"), -1, 0);

	glUniform2f(glGetUniformLocation(textProg, "textScale"), textScaleX, textScaleY);

	glUniform2f(glGetUniformLocation(textProg, "tex_size"), textureW, textureH);

	glUniform3f(glGetUniformLocation(textProg, "textColor"), textColor.x, textColor.y, textColor.z);

	glBindVertexArray(textvao);

	glDrawArrays(GL_TRIANGLES, 0, pts->size() / 4);
}

void textBox::setTextLineLength(int len)
{
	this->linlen = len;
}
