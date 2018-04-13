#include "toggle.h"



toggle::toggle(GLuint p, float dims[4],GLint base, GLint active)
	: BaseBox::BaseBox(p, dims, glm::vec4(1))
{
	this->active = active;
	this->base = base;
	this->setUIcallBack(togglecallback, this);
	this->setTexture(this->state ? this->active : this->base,1,1);
	
}

void toggle::click()
{
	this->state = !this->state;
	this->setTexture(this->state ? this->active : this->base, 1, 1);
}


toggle::~toggle()
{
}

void toggle::setBaseTex(GLint base)
{
	if (!state)
		setTexture(base,1,1);
	this->base = base;
}

void toggle::setActiveTex(GLint active)
{
	if (state)
		setTexture(active,1,1);
	this->active = active;
}

int togglecallback(glm::vec2 mouse, int button, char key, int mods, void* point) {
	((toggle*)point)->click();
	return 1;
}
