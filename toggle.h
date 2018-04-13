#pragma once
#include "BaseBox.h"

int togglecallback(glm::vec2 mouse, int button, char key, int mods, void* point);
class toggle :
	public BaseBox
{
public:
	toggle(GLuint p, float dims[4],GLint base, GLint active);
	void click();
	int state = 0;
	~toggle();
	void setBaseTex(GLint);
	void setActiveTex(GLint);
private:
	GLint base, active;
};

