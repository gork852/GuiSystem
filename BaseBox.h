#pragma once
#include "glmincluder.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_image.h>
#include <gl/GL.h>
#include <gl/glu.h>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H  

#define  BOARDER (1)
#define  ALWAYSDIRTY  (1 << 1)
#define  DISCARD_ALPHA (1 << 2)



class BaseBox
{
public:
	BaseBox(GLuint,float[4],glm::vec4);
	//BaseBox(BaseBox *p);
	~BaseBox();

	BaseBox();

	void clean();

	void setWidth(float w);
	void setHeight(float h);
	void setInnerWidth(float w);
	void setInnerHeight(float h);

	void setRelitivePos(float, float);
	void setParentPos(float, float);

	void setColors(glm::vec4[4]);
	void setColors(glm::vec4);

	void setBoarderWidth(float[4]);
	void setBoarderWidth(float);

	void setBoarderRound(float);
	void setBoarderRound(float[4]);

	void setBoarderColors(glm::vec4[4]);
	void setBoarderColors(glm::vec4);

	void setTexture(GLint , int , int );

	int getOptions();
	void setOptions(int opt);

	void draw();

	void setChild(BaseBox*);
	std::vector<BaseBox*>* getChildren();
	void removeChild(int rem);
	void setDirty();

	std::vector<glm::vec4>* boxPoints();
	std::vector<glm::vec4>* boxCoords();
	std::vector<glm::vec4>* boxBoarders();
	std::vector<glm::vec4>* boxColors();
	std::vector<glm::vec4>* boxBoarderColors();

	void createAlternateRenderTarget(int width, int height);
	void setAlternateRenderCallback(void(* func)(void*), void* point);
	void renderAlternate();

	float getHeight();
	float getWidth();

	int processUIinput(glm::vec2 mouse, int button, char key, int mods);

	void setUIcallBack(int(*ui)(glm::vec2,int,char,int,void*),void* point);
private:
	float h, w, ih, iw;
	float relX, relY;
	float parentX, parentY;
	glm::vec4 color[4];
	float boarder[4];
	float round[4];
	glm::vec4 boarderColor[4];
	int opts;
	bool dirty;
	std::vector<BaseBox*>* children;
	BaseBox *parrent = NULL;

	bool useShadeBox = false;
	GLuint renderTextureId;
	GLuint rboId;
	GLuint fboId;
	void (* altCallback)(void*) = NULL;
	void* callbackPointer;

	void* UIpoint;
	int(*UIcallback)(glm::vec2 mouse, int button, char key, int mods, void* point) = NULL;

	bool texture = false;
	GLuint tex;
	int texture_Width;
	int texture_height;

	//ogl info
	GLuint prog;
	GLuint boxVAO;

	GLuint LocationVbo;
	GLuint rectCoordVbo;
	GLuint boarderVbo;
	GLuint colorVbo, borderColorVbo;
	//////////////////////////////////////////////////////////////////////
	//					.   coord(0,1,0)boarder(.1,.1,0) last of boarder is rounding
	//				   .   .
	//				  .       .
	//				 .			 .
	//				.               .
	//			   .				   .
	//			  .					X	  .   coord(.9,.485,0)boarder(.0001,.0999,0)
	//			 .							 .
	//          .								.
	//		   .								   .
	//        .										  .  coord(1,1,0)boarder(0,.1,0)
	//		 .									  .	 
	//      .								.
	//	   .					     .
	//	  .                   .
	//   .              .
	//  .      .
	// .   coord(1,0,0)boarder(.1,0,0)
	//
	//
	//
	//
	//////////////////////////////////////////////////////////////////////////
};