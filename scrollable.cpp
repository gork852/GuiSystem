#include "scrollable.h"


void renderCalbackDref(void* point) {
	((scrollable*)point)->renderScrollCallback();
}
int upScrollCall(glm::vec2 mouse, int button, char key, int mods, void* point) {
	if (button) {
		((scrollable*)point)->scroll(1);
	}
	return 1;
}
int downScrollCall(glm::vec2 mouse, int button, char key, int mods, void* point) {
	if (button) {
		((scrollable*)point)->scroll(-1);
	}
	return 1;
}


scrollable::scrollable(GLuint p, float dims[4], float scrollWidth)
	: BaseBox::BaseBox(p, dims, glm::vec4(.3, .3, .3, 1))
{
	this->prog = p;
	this->scrollWidth = scrollWidth;
	createAlternateRenderTarget(1280,720);
	setAlternateRenderCallback(renderCalbackDref,this);
	float dimss[4];
	dimss[0] = scrollWidth;
	dimss[1] = scrollWidth;
	dimss[2] = 1;
	dimss[3] = 1;


	glm::vec4 color = glm::vec4(.3, .3, .3, 1);
	color[0] = 1;

	this->up = new BaseBox(p, dimss, color);
	this->up->setRelitivePos(1 - scrollWidth, 0);
	this->up->setUIcallBack(upScrollCall, this);

	this->down = new BaseBox(p, dimss, color);
	this->down->setRelitivePos(1 - scrollWidth, 1-scrollWidth);
	this->down->setUIcallBack(downScrollCall, this);

	color[0] = 1;

	this->barHeight = .2;

	color[1] = 1;
	dimss[1] = this->barHeight;

	this->bar = new BaseBox(p, dimss, color);
	this->setRate( .01);
	this->bar->setRelitivePos(1 - scrollWidth, .5- this->barHeight / 2);

	color[0] = .3;
	color[1] = .3;
	color[2] = 1;

	dimss[1] = 1;

	this->scrollSide = new BaseBox(p, dimss, color);
	this->scrollSide->setRelitivePos(1 - scrollWidth, 0);
	this->setUIcallBack(contentCaller,this);
}


scrollable::~scrollable()
{
}


void scrollable::addContent(BaseBox* b)
{
	this->content.push_back(b);
	
}

void scrollable::setRate( float rate)
{
	this->scrollRate = rate;
}

void scrollable::renderScrollCallback()
{
	this->updateContentPos();
	glUseProgram(this->prog);
	for (int i = 0; i < this->content.size(); i++) {
		glUseProgram(this->prog);
		this->content[i]->draw();
	}
	this->scrollSide->draw();
	this->bar->draw();
	this->up->draw();
	this->down->draw();
}

void scrollable::scroll(float dir)
{
	this->targetScroll += dir*this->scrollRate;
}

float scrollable::getContentHeight()
{
	float t=0;
	for (int i = 0; i < this->content.size(); i++) {
		t += this->content[i]->getHeight();
	}
	return t;
}

float scrollable::getContentWidth()
{
	float t = 0;
	for (int i = 0; i < this->content.size(); i++) {
		float w = this->content[i]->getWidth();
		t = t > w ? t : w;//old width larger than new, keep old else use new
	}
	return t;
}

void scrollable::updateContentPos()
{
	if (targetScroll > getContentHeight() - 1)targetScroll = getContentHeight() - 1;
	if (targetScroll < 0) targetScroll = 0;
	this->currentScroll = (this->currentScroll + this->targetScroll) / 2;
	float startHeight = 1+this->currentScroll;
	for (int i = 0; i < this->content.size(); i++) {
		startHeight -= this->content[i]->getHeight();
		this->content[i]->setRelitivePos(0, startHeight);
	}
	float scrollPercent = this->currentScroll / (this->getContentHeight() - 1);
	this->bar->setRelitivePos(1 - this->scrollWidth,
		(1-scrollPercent)*(1-barHeight*2)+scrollWidth );
	//printf("the scrollpercent is %f\n", scrollPercent);
}

int contentCaller(glm::vec2 mouse, int button, char key, int mods,void* point) {
	return ((scrollable*)point)->contentUIprocess(mouse, button, key, mods);
}

int scrollable::contentUIprocess(glm::vec2 mouse, int button, char key, int mods)
{
	int t = 0;
	t |= this->up->processUIinput(mouse, button, key, mods);
	t |= this->down->processUIinput(mouse, button, key, mods);
	t |= this->bar->processUIinput(mouse, button, key, mods);
	for (int i = 0; i < this->content.size(); i++) {
		t |= this->content[i]->processUIinput(mouse, button, key, mods);
	}
	return t;
}
