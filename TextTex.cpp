#define _CRT_SECURE_NO_WARNINGS
#include "TextTex.h"

TextTex::TextTex(char* defSrc)
{
	FILE* f = fopen(defSrc, "r");
	//printf("%d was f\n", f);
	char buf[200];
	//discard first 2 lines
	fgets(buf, 200, f);
	fgets(buf, 200, f);
	//find height
	fgets(buf, 200, f);
	sscanf(buf, "\theight=%d,\n", &this->height);
	//printf("got %d as height\n", this->height);
	//this->height*= scale;
	//discard description
	fgets(buf, 200, f);
	fgets(buf, 200, f);
	fgets(buf, 200, f);
	fgets(buf, 200, f);
	fgets(buf, 200, f);
	//get asc and des
	fgets(buf, 200, f);
	fgets(buf, 200, f);
	//sscanf(buf, " % 13 * %d", this->ascender);
	sscanf(buf, "		ascender = %d,\n", &this->ascender);
	//printf("got %d as ascender\n", this->ascender);
	//this->ascender *= scale;
	fgets(buf, 200, f);
	//sscanf(buf, "%14*%d", this->descender);
	sscanf(buf, "		descender = %d,\n", &this->descender);
	//printf("got %d as descender\n", this->descender);
	//this->descender *= scale;
	fgets(buf, 200, f);
	fgets(buf, 200, f);
	//get tex h and w
	fgets(buf, 200, f);
	fgets(buf, 200, f);
	fgets(buf, 200, f);
	//sscanf(buf, "%8*%d", this->texH);
	sscanf(buf, "		width=%d,\n", &this->texH);
	//printf("got %d as texH\n", this->texH);
	
	fgets(buf, 200, f);
	//sscanf(buf, "%9*%d", this->texW);
	sscanf(buf, "		height=%d,\n", &this->texW);
	//printf("got %d as texW\n", this->texW);
	
	fgets(buf, 200, f);
	//get char array
	fgets(buf, 200, f);
	fgets(buf, 200, f);
	while (buf[1]!='}') {
		
		char c = buf[9];
		//sscanf(buf, "%*18%d%3*%d%3*%d%3*%d%3*%d%4*%d%4*%d", this->chars[c].width, this->chars[c].x, this->chars[c].y, this->chars[c].w, this->chars[c].h, this->chars[c].ox, this->chars[c].oy);
		if (strrchr(buf, '\\')) 
			sscanf(buf, "		{char=%*c%*c%*c%*c,width=%d,x=%d,y=%d,w=%d,h=%d,ox=%d,oy=%d},\n", &this->chars[c].width, &this->chars[c].x, &this->chars[c].y, &this->chars[c].w, &this->chars[c].h, &this->chars[c].ox, &this->chars[c].oy);
		else
			sscanf(buf, "		{char=%*c%*c%*c,width=%d,x=%d,y=%d,w=%d,h=%d,ox=%d,oy=%d},\n", &this->chars[c].width, &this->chars[c].x, &this->chars[c].y, &this->chars[c].w, &this->chars[c].h, &this->chars[c].ox, &this->chars[c].oy);
		//printf("scanned  <%c> with %d %d %d %d %d %d %d\n",c, this->chars[c].width, this->chars[c].x, this->chars[c].y, this->chars[c].w, this->chars[c].h, this->chars[c].ox, this->chars[c].oy);
		fgets(buf, 200, f);

		/*this->chars[c].h *= scale;
		this->chars[c].ox *= scale;
		this->chars[c].oy *= scale;
		this->chars[c].w *= scale;
		this->chars[c].width *= scale;
		this->chars[c].x *= scale;
		this->chars[c].y *= scale;*/
	}
	fclose(f);
	
		

}

int TextTex::texWidth(char * txt)
{
	int len = 0;
	while (*txt != NULL) {
		len += this->chars[*txt].width;
		txt++;
	}
	return len;
}

int TextTex::texHeight(int limit, char * txt)
{
	int height = this->height;
	int len = 0;
	while (*txt != NULL) {
		int tmp = this->chars[*txt].width;
		if (len + tmp > limit) {
			len = tmp;
			height += this->ascender;
		}
		else {
			len += tmp;
		}
		txt++;
	}
	return height;
}

std::vector<float>* TextTex::TextToTex(char * strr)
{
	//format
	// 24 floats per quad
	// 12 floats per tri
	// 6 floats Position (xy) + 6 floats Tex coords
	// |------tri1-------||------tri2-------||-----tri3...
	// |---------------quad1----------------||-----quad2...
	// |xy tt xy tt xy tt||xy tt xy tt xy tt|
	std::vector<float>* out = new std::vector<float>();
	float curX = 0;
	float curY = 0;
	for (int i = 0; i < strlen(strr); i++) {
		//std::vector<float>* bx = this->MakeQuad(curX + this->chars[strr[i]].ox, curY + this->chars[strr[i]].oy, this->chars[strr[i]].w, this->chars[strr[i]].h, this->chars[strr[i]].x, this->chars[strr[i]].y);
		std::vector<float>* bx = this->MakeQuad(curX, curY ,this->chars[strr[i]].w, this->chars[strr[i]].h, this->chars[strr[i]].x, this->chars[strr[i]].y);
		out->insert(out->end(), bx->begin(), bx->end());
		curX += this->chars[strr[i]].width;
	}

	return out;
}



std::vector<float>* TextTex::TextToTex(int maxWidth, char * strr)
{
	std::vector<float>* out = new std::vector<float>();
	float curX = 0;
	float curY = 0;
	for (int i = 0; i < strlen(strr); i++) {
		if (curX + this->chars[strr[i]].width> maxWidth) {
			curY -= this->ascender;
			curX = 0;
		}
		//std::vector<float>* bx = this->MakeQuad(curX + this->chars[strr[i]].ox, curY + this->chars[strr[i]].oy, this->chars[strr[i]].w, this->chars[strr[i]].h, this->chars[strr[i]].x, this->chars[strr[i]].y);
		std::vector<float>* bx = this->MakeQuad(curX, curY, this->chars[strr[i]].w, this->chars[strr[i]].h, this->chars[strr[i]].x, this->chars[strr[i]].y);
		out->insert(out->end(), bx->begin(), bx->end());
		curX += this->chars[strr[i]].width;
	}

	return out;
}

std::vector<float>* TextTex::MakeQuad(float x, float y, float w, float h, float tx, float ty) {
	std::vector<float>* points = new std::vector<float>();
	/*
	UL___UR
	|\   |
	| \  |
	|  \ |
	LL__\LR
	*/


	//lower left tri
	//LL point
	points->push_back(x);
	points->push_back(y);

	points->push_back(tx);
	points->push_back(ty + h);

	//UL point
	points->push_back(x);
	points->push_back(y + h);

	points->push_back(tx);
	points->push_back(ty);
	

	//LR point
	points->push_back(x + w);
	points->push_back(y);

	points->push_back(tx + w);
	points->push_back(ty + h);

	//uper right tri
	//UR point
	points->push_back(x + w);
	points->push_back(y + h);

	points->push_back(tx + w);
	points->push_back(ty);
	
	//UL point
	points->push_back(x);
	points->push_back(y + h);

	points->push_back(tx);
	points->push_back(ty);
	//LR point
	points->push_back(x + w);
	points->push_back(y);

	points->push_back(tx + w);
	points->push_back(ty + h);

	
	
	return points;
}

TextTex::~TextTex()
{
}
