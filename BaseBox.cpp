#include "BaseBox.h"



BaseBox::BaseBox(GLuint p,float dims[4], glm::vec4 color)
{
	this->children = new std::vector<BaseBox*>();
	this->prog = p;
	this->w = dims[0];
	this->h = dims[1];
	this->iw = dims[2];
	this->ih = dims[3];
	this->setColors(color);

	glGenVertexArrays(1, &this->boxVAO);

	glGenBuffers(1, &this->LocationVbo);
	glGenBuffers(1, &this->rectCoordVbo);
	glGenBuffers(1, &this->boarderVbo);
	glGenBuffers(1, &this->colorVbo);
	glGenBuffers(1, &this->borderColorVbo);

	//this->dirty = true;
	
	
	this->setDirty();
}

/*BaseBox::BaseBox(BaseBox * p)
{

}
*/

BaseBox::~BaseBox()
{
	//gl cleanup :)

	//memcleanup
	for (int i = 0; i < this->children->size(); i++) {
		this->children->at(i)->~BaseBox();
	}
}

BaseBox::BaseBox()
{
	printf("this should never be called\n");
	exit(-1);
}

void BaseBox::clean()
{
	if (this->parrent && this->parrent->dirty) {
		this->parrent->clean();
	}
	if(!(opts&ALWAYSDIRTY))
		dirty = false;

	//printf("glerr %d at %d\n", glGetError(), __LINE__);

	float* points = glm::value_ptr(boxPoints()->data()[0]);
	glBindBuffer(GL_ARRAY_BUFFER, this->LocationVbo);
	//two triangles
	//three points per triangle
	//three floats per point
	//sizeof of float per float
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 4 * sizeof(float), points, GL_STATIC_DRAW);
	
	points = glm::value_ptr(boxCoords()->data()[0]);
	glBindBuffer(GL_ARRAY_BUFFER, this->rectCoordVbo);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 4 * sizeof(float), points, GL_STATIC_DRAW);
	 
	points = glm::value_ptr(boxBoarders()->data()[0]);
	glBindBuffer(GL_ARRAY_BUFFER, this->boarderVbo);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 4 * sizeof(float), points, GL_STATIC_DRAW);

	points = glm::value_ptr(boxColors()->data()[0]);
	glBindBuffer(GL_ARRAY_BUFFER, this->colorVbo);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 4 * sizeof(float), points, GL_STATIC_DRAW);

	points = glm::value_ptr(boxBoarderColors()->data()[0]);
	glBindBuffer(GL_ARRAY_BUFFER, this->borderColorVbo);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 4 * sizeof(float), points, GL_STATIC_DRAW);

	glBindVertexArray(this->boxVAO);

	glEnableVertexAttribArray(glGetAttribLocation(this->prog, "box_point_pos"));
	glBindBuffer(GL_ARRAY_BUFFER, this->LocationVbo);
	glVertexAttribPointer(glGetAttribLocation(this->prog, "box_point_pos"), 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(glGetAttribLocation(this->prog, "box_point_cords"));
	glBindBuffer(GL_ARRAY_BUFFER, this->rectCoordVbo);
	glVertexAttribPointer(glGetAttribLocation(this->prog, "box_point_cords"), 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(glGetAttribLocation(this->prog, "box_point_color"));
	glBindBuffer(GL_ARRAY_BUFFER, this->colorVbo);
	glVertexAttribPointer(glGetAttribLocation(this->prog, "box_point_color"), 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(glGetAttribLocation(this->prog, "box_boarder_amt"));
	glBindBuffer(GL_ARRAY_BUFFER, this->boarderVbo);
	glVertexAttribPointer(glGetAttribLocation(this->prog, "box_boarder_amt"), 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(glGetAttribLocation(this->prog, "box_boarder_color"));
	glBindBuffer(GL_ARRAY_BUFFER, this->borderColorVbo);
	glVertexAttribPointer(glGetAttribLocation(this->prog, "box_boarder_color"), 4, GL_FLOAT, GL_FALSE, 0, NULL);
	//printf("glerr %d at %d\n", glGetError(), __LINE__);

	for (int i = 0; i < this->children->size(); i++) {
		float maxWidth = this->parrent ? this->parrent->w : 1;
		float maxHeight = this->parrent ? this->parrent->h : 1;
		
		this->children->at(i)->setParentPos(this->parentX + (this->relX + 0)*maxWidth,
											this->parentY + (this->relY + 0)*maxHeight);
		//this->children->at(i)->setParentPos(this->relX, this->relY);
	}
}

void BaseBox::setWidth(float w)
{
	this->w = w;
	this->dirty = true;
	this->setDirty();
}

void BaseBox::setHeight(float h)
{
	this->h = h;
	this->dirty = true;
	this->setDirty();
}

void BaseBox::setInnerWidth(float w)
{
	this->iw = w;
	this->dirty = true;
	this->setDirty();
}

void BaseBox::setInnerHeight(float h)
{
	this->ih = h;
	this->dirty = true;
	this->setDirty();
}

void BaseBox::setRelitivePos(float x, float y)
{
	this->relX = x;
	this->relY = y;
	this->dirty = true;
	this->setDirty();
}

void BaseBox::setParentPos(float x, float y) {
	this->parentX = x;
	this->parentY = y;
	this->dirty = true;
	this->setDirty();
}

void BaseBox::setColors(glm::vec4 col)
{
	this->color[0] = col;
	this->color[1] = col;
	this->color[2] = col;
	this->color[3] = col;
	this->dirty = true;
	this->setDirty();
}

void BaseBox::setColors(glm::vec4 col[4])
{
	this->color[0] = col[0];
	this->color[1] = col[1];
	this->color[2] = col[2];
	this->color[3] = col[3];
	this->dirty = true;
	this->setDirty();
}

void BaseBox::setBoarderWidth(float b[4])
{
	this->boarder[0] = b[0];
	this->boarder[1] = b[1];
	this->boarder[2] = b[2];
	this->boarder[3] = b[3];
	this->dirty = true;
	this->setDirty();
}

void BaseBox::setBoarderWidth(float b)
{
	this->boarder[0] = b;
	this->boarder[1] = b;
	this->boarder[2] = b;
	this->boarder[3] = b;
	this->dirty = true;
	this->setDirty();
}

void BaseBox::setBoarderRound(float r)
{
	this->round[0] = r;
	this->round[1] = r;
	this->round[2] = r;
	this->round[3] = r;
	this->dirty = true;
	this->setDirty();
}

void BaseBox::setBoarderRound(float r[4])
{
	this->round[0] = r[0];
	this->round[1] = r[1];
	this->round[2] = r[2];
	this->round[3] = r[3];
	this->dirty = true;
	this->setDirty();
}

void BaseBox::setBoarderColors(glm::vec4 cols[4])
{
	this->boarderColor[0] = cols[0];
	this->boarderColor[1] = cols[1];
	this->boarderColor[2] = cols[2];
	this->boarderColor[3] = cols[3];
	this->dirty = true;
	this->setDirty();
}

void BaseBox::setBoarderColors(glm::vec4 col)
{
	this->boarderColor[0] = col;
	this->boarderColor[1] = col;
	this->boarderColor[2] = col;
	this->boarderColor[3] = col;
	this->dirty = true;
	this->setDirty();
}

int BaseBox::getOptions()
{
	return opts;
}

void BaseBox::setOptions(int opt)
{
	opts = opt;
}

void BaseBox::setTexture(GLint tex, int width, int height) {
	this->tex = tex;
	this->texture_Width = width;
	this->texture_height = height;
	this->texture = true;
	if (tex == -1) {
		this->texture = false;
	}
}

void BaseBox::draw()
{
	if (this->dirty == true)
		this->clean();
	//move to VAO if possible
	GLint vet_opt = glGetUniformLocation(this->prog, "options");
	glUniform1i(vet_opt, this->opts);
	if (this->texture || this->useShadeBox) {
		if(!this->useShadeBox)
			glBindTexture(GL_TEXTURE_2D, this->tex);
		else
			glBindTexture(GL_TEXTURE_2D, this->renderTextureId);
		//printf("glerr %d at %d\n", glGetError(), __LINE__);
		/*GLint temio = glGetUniformLocation(this->prog, "textureId");
		printf("glerr %d at %d\n", glGetError(), __LINE__);
		printf("%d is tem\n", temio);
		glUniform1i(temio,GL_TEXTURE_2D);*/
		//printf("glerr %d at %d\n", glGetError(), __LINE__);
		GLint frag_tex = glGetUniformLocation(this->prog, "doTexture");
		glUniform1i(frag_tex, 1);
		GLint frag_width = glGetUniformLocation(this->prog, "Twidth");
		glUniform1i(frag_width, this->useShadeBox ? 1 : this->texture_Width);
		GLint frag_height = glGetUniformLocation(this->prog, "Theight");
		glUniform1i(frag_height, this->useShadeBox ? 1 : this->texture_height);
		//printf("glerr %d at %d\n", glGetError(), __LINE__);
	}
	else {
		GLint frag_tex = glGetUniformLocation(this->prog, "doTexture");
		glUniform1i(frag_tex, -1);
	}

	//draw
	glBindVertexArray(this->boxVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//printf("glerr %d at %d\n", glGetError(), __LINE__);
	
	for (int i = 0; i < this->children->size(); i++){
		this->children->at(i)->draw();
	}
}

void BaseBox::setChild(BaseBox *c)
{
	this->children->push_back(c);
	c->parrent = this;
	this->dirty = true;
	c->dirty = true;
	this->setDirty();
}

std::vector<BaseBox*>* BaseBox::getChildren()
{
	return this->children;
}

void BaseBox::removeChild(int rem) {
	this->children->erase(this->children->begin() + rem);
}

////////////upsie-down////////////////////
//
//	[0]	.	.	.	.	.	.	.	.	[1]
//		.
//  .		.							.
//				.
//  .				 .					.
//						  .
//  .							.		.
//									.
//  [2]	.	.	.	.	.	.	.	.	[3]
//
//
/////////////////////////////////////////////////

std::vector<glm::vec4>* BaseBox::boxPoints()
{
	//printf("vars relx %f, parX %f\n", this->relX, this->parentX);
	//printf("vars rely %f, parY %f\n", this->relY, this->parentY);
	
	std::vector<glm::vec4> *points = new std::vector<glm::vec4>();;
	float maxWidth = this->parrent ? this->parrent->w : 1;
	float maxHeight = this->parrent ? this->parrent->h : 1;
	points->push_back(glm::vec4(this->parentX + (this->relX + 0)*maxWidth,
								this->parentY + (this->relY + 0)*maxHeight,
								0, 1));
	points->push_back(glm::vec4(this->parentX + (this->relX + this->w)*maxWidth,
								this->parentY + (this->relY + 0)*maxHeight,
								0, 1));
	points->push_back(glm::vec4(this->parentX + (this->relX + this->w)*maxWidth,
								this->parentY + (this->relY + this->h)*maxHeight,
								0, 1));


	points->push_back(glm::vec4(this->parentX + (this->relX + 0)*maxWidth,
								this->parentY + (this->relY + 0)*maxHeight,
								0, 1));
	points->push_back(glm::vec4(this->parentX + (this->relX + 0)*maxWidth,
								this->parentY + (this->relY + this->h)*maxHeight,
								0, 1));
	points->push_back(glm::vec4(this->parentX + (this->relX + this->w)*maxWidth,
								this->parentY + (this->relY + this->h)*maxHeight,
								0, 1));

	return points;
}

std::vector<glm::vec4>* BaseBox::boxCoords()
{
	std::vector<glm::vec4> *points = new std::vector<glm::vec4>();;

	points->push_back(glm::vec4(0, 0, 0, 0));
	points->push_back(glm::vec4(1, 0, 0, 0));
	points->push_back(glm::vec4(1, 1, 0, 0));

	points->push_back(glm::vec4(0, 0, 0, 0));
	points->push_back(glm::vec4(0, 1, 0, 0));
	points->push_back(glm::vec4(1, 1, 0, 0));

	return points;
}

std::vector<glm::vec4>* BaseBox::boxBoarders()
{
	std::vector<glm::vec4> *points = new std::vector<glm::vec4>();
	
	points->push_back(glm::vec4(this->boarder[0], 0, 0, this->round[0]));
	points->push_back(glm::vec4(this->boarder[1], this->boarder[1], 0, this->round[1]));
	points->push_back(glm::vec4(0, this->boarder[3], 0, this->round[3]));

	points->push_back(glm::vec4(this->boarder[0], 0, 0, this->round[0]));
	points->push_back(glm::vec4(this->boarder[2], this->boarder[2], 0, this->round[2]));
	points->push_back(glm::vec4(0, this->boarder[3], 0, this->round[3]));

	return points;
}

std::vector<glm::vec4>* BaseBox::boxColors()
{
	std::vector<glm::vec4> *points = new std::vector<glm::vec4>();;

	points->push_back(this->color[0]);
	points->push_back(this->color[1]);
	points->push_back(this->color[3]);

	points->push_back(this->color[0]);
	points->push_back(this->color[2]);
	points->push_back(this->color[3]);

	return points;
}

std::vector<glm::vec4>* BaseBox::boxBoarderColors()
{
	std::vector<glm::vec4> *points = new std::vector<glm::vec4>();;

	points->push_back(this->boarderColor[0]);
	points->push_back(this->boarderColor[1]);
	points->push_back(this->boarderColor[3]);

	points->push_back(this->boarderColor[0]);
	points->push_back(this->boarderColor[2]);
	points->push_back(this->boarderColor[3]);

	return points;
}

void BaseBox::setDirty() {
	this->dirty = true;
	//printf( "here %d\n",this->children->size());
	for (int i = 0; i < this->children->size(); i++) {
		//printf("why/n");
		this->children->at(i)->setDirty();
	}
	//printf("here\n");
}

void BaseBox::renderAlternate() {
	/*for (int i = 0; i < this->children->size(); i++) {
		this->children->at(i)->renderAlternate();
	}*/
	if (this->useShadeBox) {
		//printf("glerr %d at %d\n", glGetError(), __LINE__);
		glBindFramebuffer(GL_FRAMEBUFFER, this->fboId);
		//printf("glerr %d at %d\n", glGetError(), __LINE__);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//printf("glerr %d at %d\n", glGetError(), __LINE__);
		this->altCallback(this->callbackPointer);
		//printf("glerr %d at %d\n", glGetError(), __LINE__);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//printf("glerr %d at %d\n", glGetError(), __LINE__);
		glBindTexture(GL_TEXTURE_2D, this->renderTextureId);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		//printf("glerr %d at %d\n", glGetError(), __LINE__);

	}
}

float BaseBox::getHeight()
{
	return this->h;
}

float BaseBox::getWidth()
{
	return this->w;
}

int BaseBox::processUIinput(glm::vec2 mouse, int button, char key, int mods)
{
	//printf("hello mx %f my %f relx %f w %f rely %f h %f\n",mouse[0],mouse[1],this->relX,this->w,this->relY,this->h);
	if (mouse[0] > this->relX && mouse[0]<this->relX + this->w && mouse[1]>this->relY && mouse[1] < this->relY + this->h) {
		//printf("win\n");
		mouse[0] = (mouse[0] - relX) / this->w;
		mouse[1] = (mouse[1] - relY) / this->h;
		if(UIcallback)
			this->UIcallback(mouse, button, key, mods, this->UIpoint);
		for (int i = 0; i < this->children->size(); i++) {
			//printf("inside\n");
			this->children->at(i)->processUIinput(mouse, button, key, mods);
		}
		return 1;
	}
	return 0;
}

void BaseBox::setUIcallBack(int(*ui)(glm::vec2, int, char, int, void*),void* point)
{
	UIcallback = ui;
	this->UIpoint = point;
}

void BaseBox::setAlternateRenderCallback(void(* func)(void*),void* point) {
	this->altCallback = func;
	this->useShadeBox = true;
	this->callbackPointer = point;
}

void BaseBox::createAlternateRenderTarget(int width, int height)
{
	//http://www.songho.ca/opengl/gl_fbo.html#example	
	glGenTextures(1, &(this->renderTextureId));
	glBindTexture(GL_TEXTURE_2D, this->renderTextureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &(this->rboId));
	glBindRenderbuffer(GL_RENDERBUFFER, this->rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &(this->fboId));
	glBindFramebuffer(GL_FRAMEBUFFER, this->fboId);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER 
		GL_COLOR_ATTACHMENT0,  // 2. attachment point
		GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
		this->renderTextureId,             // 4. tex ID
		0);                    // 5. mipmap level: 0(base)

	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
		GL_DEPTH_ATTACHMENT, // 2. attachment point
		GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
		this->rboId);              // 4. rbo ID

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("altRenderFailXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
