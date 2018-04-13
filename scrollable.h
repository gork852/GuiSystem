
#include "BaseBox.h"


class scrollable :
	public BaseBox
{
public:
	scrollable(GLuint p, float dims[4], float scrollWidth);
	~scrollable();
	void addContent(BaseBox* b);
	void setRate(float rate);
	void renderScrollCallback();
	void scroll(float dir);
	BaseBox* scrollSide;
	BaseBox* up;
	BaseBox* down;
	BaseBox* bar;
	std::vector<BaseBox*> content;
	int contentUIprocess(glm::vec2 mouse, int button, char key, int mods);

private:
	
	float scrollWidth;
	float arrowHeight;
	float barHeight;
	float currentScroll=0;
	float targetScroll=0;
	float scrollRate;

	float getContentHeight();
	float getContentWidth();
	void updateContentPos();
	

	GLint prog;

	
};

int contentCaller(glm::vec2 mouse, int button, char key, int mods, void * point);
