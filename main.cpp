#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <gl/GL.h>
#include <gl/glu.h>
#include <math.h>
#include <vector>
//Not familiar with Allegro? try this link! http://liballeg.org/a5docs/5.0.10/getting_started.html

//add the gl_extention!
#include <allegro5/allegro_opengl.h>
//glm library headers
#include "glmincluder.h"

//reference-> http://liballeg.org/a5docs/5.0.10/opengl.html
////////////////////////////////
#include "shader.h"
#include "camera.h"
#include "BaseBox.h"
#include "scrollable.h"
#include "toggle.h"
#include "TextTex.h"
#include "textBox.h"
#include "main.h"



#define HWTriangle 0
ALLEGRO_DISPLAY *display = NULL;
int screen_width = set_g_width(1280);
int screen_height = set_g_height(720);
FT_Library library;//free type library
FT_Face face;

ALLEGRO_DISPLAY* createOpenGlDisplay(){
	al_set_new_display_flags(ALLEGRO_OPENGL);
	ALLEGRO_DISPLAY *display = al_create_display(get_g_width(), get_g_height());
	return display;
}

GLuint worldGroundVbo;
GLuint worldGroundVao;
GLuint worldNormalVbo;
int frameCount = 0;

void initWorldGround(GLint prog) {
	float points[] = {
		0.0f,  0.5f,  0.0f,
		0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  -0.5f
	};
	
	float normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};

	glGenBuffers(1, &worldGroundVbo);
	glBindBuffer(GL_ARRAY_BUFFER, worldGroundVbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	glGenBuffers(1, &worldNormalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, worldNormalVbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), normals, GL_STATIC_DRAW);


	glGenVertexArrays(1, &worldGroundVao);
	glBindVertexArray(worldGroundVao);

	glEnableVertexAttribArray(glGetAttribLocation(prog, "vertPos"));

	glBindBuffer(GL_ARRAY_BUFFER, worldGroundVbo);

	glVertexAttribPointer(glGetAttribLocation(prog, "vertPos") , 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(glGetAttribLocation(prog, "vertNor"));
	glBindBuffer(GL_ARRAY_BUFFER, worldNormalVbo);
	glVertexAttribPointer(glGetAttribLocation(prog, "vertNor"), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void renderWorld(GLint shader_programme) {

	//int i = 800;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_programme);

	//find vert uniforms
	GLint vert_P = glGetUniformLocation(shader_programme, "P");
	GLint vert_MV = glGetUniformLocation(shader_programme, "MV");
	GLint vert_World = glGetUniformLocation(shader_programme, "World");
	GLint vert_vLight = glGetUniformLocation(shader_programme, "vLight");

	//find frag uniforms
	GLint vert_vColor = glGetUniformLocation(shader_programme, "vColor");

	//set up projection matrix
	glm::fmat4x4 P =	glm::perspectiveFov<float>(90.0, get_g_width(), get_g_height(), .1, 100);

	//printf("\n\n %d %d \n\n", get_g_width(), get_g_height());
	glUniformMatrix4fv(vert_P, 1, false, glm::value_ptr(P));

	//set ground in its place
	glm::fmat4x4 MV = glm::mat4x4(1);
	
	MV = glm::translate(MV, glm::vec3(0, 0, -2));
	MV = glm::rotate(MV, frameCount++/30.0f, glm::vec3(0, 1, 0));

	glm::fmat4x4 World = glm::mat4x4(1);
	glm::vec3 vLight = glm::normalize(glm::vec3(0, 2, 4));

	glUniformMatrix4fv(vert_MV, 1, false, glm::value_ptr(MV));

	glUniformMatrix4fv(vert_World, 1, false, glm::value_ptr(World));

	glUniform3f(vert_vLight, vLight[0], vLight[1], vLight[2]);
	
	//set color to redish
	glUniform4f(vert_vColor, .9, .5, .6, 1);

	glBindVertexArray(worldGroundVao);
	// draw points 0-3 from the currently bound VAO with current in-use shader
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	//printf("glerr %d at %d\n", glGetError(),__LINE__);

	//char charr;
	//scanf("%c",&charr);
}

GLuint UIBaseVbo;
GLuint UIBaseVao;
BaseBox *box = NULL;
BaseBox *bbox = NULL;
BaseBox *deep = NULL;
BaseBox *textbox = NULL;
BaseBox *shadeBox = NULL;

//ALLEGRO_FONT* fnt;
/*
ALLEGRO_FONT* fnt;
ALLEGRO_BITMAP *create_text_render(ALLEGRO_FONT *font, ALLEGRO_COLOR color, const char *text)
{
	ALLEGRO_STATE previous_state;
	al_store_state(&previous_state, ALLEGRO_STATE_TARGET_BITMAP);

	int bitmap_w = al_get_text_width(font, text);
	int bitmap_h = al_get_font_line_height(font);
	ALLEGRO_BITMAP *text_render = al_create_bitmap(bitmap_w, bitmap_h);

	al_set_target_bitmap(text_render);
	al_clear_to_color(al_map_rgba_f(1.0, 0.0, 0.0, 0.0));

	al_draw_text(font, al_map_rgb(0,255,0), 0, 0, ALLEGRO_ALIGN_CENTRE, "aaaaa");
	//al_draw_glyph(font, color, 0, 0, 25);
	
	

	al_restore_state(&previous_state);
	return text_render;
}
*/
TextTex* t = NULL;
float textScaleX, textScaleY;
float textureW, textureH;
GLuint textvbo;
GLuint textvao;
std::vector<float>* pts;
GLuint textProg;
GLint texture3;

GLuint altShadeTestTmp;

scrollable* scrollDemo;

toggle* toggleBox;

textBox* textBoxTest;
void ringRing(void* point) {//callback lol
	renderWorld(altShadeTestTmp);

}

void initUIBase2(GLint prog) {
	//70% relative width, 40% relative height, no internal padding
	float dims[4] = { .7,.4,1,1 };
	// create white box
	box = new BaseBox(prog, dims, glm::vec4(1, 1, 1, 1));
	// set the lower left corner of the box 10% up from the lower left corner and 5% to the right
	box->setRelitivePos(.1, .05);
	glm::vec4 cls[4] = { glm::vec4(1,1,1,1) ,
		glm::vec4(.4,.1,.4,1),
		glm::vec4(1,1,1,1),
		glm::vec4(1,1,1,1) };
	box->setColors(cls);
	glm::vec4 bcls[4] = { glm::vec4(.9,.19,.19,1) ,
		glm::vec4(.19,.19,.19,1) ,
		glm::vec4(.19,.19,.9,1) ,
		glm::vec4(.19,.9,.19,1) };
	box->setBoarderColors(bcls);
	box->setBoarderRound(.1);
	box->setBoarderWidth(.1);
}
void initUIBase(GLint prog) {
	//fnt = al_load_font("text/ostrich-regular.ttf", 36, 0);
	//fnt = al_load_font("text/OpenSans-Bold.ttf", 36, 0);
	/*float points[] = {
		0.0f,  0.5f,  0.0f,
		0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  -0.5f
	};

	glGenBuffers(1, &UIBaseVbo);
	glBindBuffer(GL_ARRAY_BUFFER, UIBaseVbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &UIBaseVao);
	glBindVertexArray(UIBaseVao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, UIBaseVbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);*/
	float dims[4] = { .7,.4,1,1 };
	//printf("glerr %d at %d\n", glGetError(), __LINE__);
	box = new BaseBox(prog, dims,glm::vec4(1,1,1,1));
	glm::vec4 cls[4] = {	glm::vec4(1,1,1,1) ,
							glm::vec4(.4,.1,.4,1) ,
							glm::vec4(1,1,1,1) ,
							glm::vec4(1,1,1,1) };
	box->setColors(cls);
	glm::vec4 bcls[4] = { glm::vec4(.9,.19,.19,1) ,
						glm::vec4(.19,.19,.19,1) ,
						glm::vec4(.19,.19,.9,1) ,
						glm::vec4(.19,.9,.19,1) };
	box->setBoarderColors(bcls);
	box->setBoarderRound(.1);
	box->setBoarderWidth(.1);
	box->setRelitivePos(.1,.05);
	
	float dimss[4] = { .5,.8,1,1 };
	bbox = new BaseBox(prog, dimss, glm::vec4(1, 1, 1, 1));
	bbox->setColors(glm::vec4(1, .2, .2, 1));
	bbox->setBoarderColors(glm::vec4(0, 0, 0, 1));
	bbox->setRelitivePos(.4, .1);
	bbox->setBoarderWidth(.1);

	ALLEGRO_BITMAP* bitmap = al_load_bitmap("images/tester.png");
	ALLEGRO_BITMAP* bitmap2 = al_load_bitmap("images/hi-rez_test.png");
	
	//ALLEGRO_BITMAP* bitmap2 = create_text_render(fnt, al_map_rgba(155, 155, 155, 155), "hello world\n");
	//printf("a %d",bitmap2);
	GLint texture = al_get_opengl_texture(bitmap2);
	//printf("b %d\n",texture);
	//GLint texture2 = al_get_opengl_texture(bitmap2);
	//al_save_bitmap("cool.bmp",bitmap2);
	//printf("rtex 2 is %d\n", texture2);
	bbox->setTexture(texture,1,1);

	float dimssss[4] = { .4,.4,1,1 };
	shadeBox = new BaseBox(prog, dimssss, glm::vec4(1, 1, 1, 1));
	shadeBox->createAlternateRenderTarget(1280, 720);
	printf("glerr %d at %d\n", glGetError(), __LINE__);
	shadeBox->setAlternateRenderCallback(ringRing,NULL);



	box->setChild(bbox);
	deep = box;
	//printf("glerr %d at %d\n", glGetError(), __LINE__);
	

	float dimsss[4] = { 1,t->texW/(t->texH*1.0),1,1 };
	ALLEGRO_BITMAP* bitmap3 = al_load_bitmap("test/anonymous_regular_65.PNG");





	//textLoadup
	char* testText = "hello world!";
	pts = t->TextToTex(300,testText);
	glGenVertexArrays(1, &textvao);
	glGenBuffers(1, &textvbo);
	glBindBuffer(GL_ARRAY_BUFFER, textvbo);
	float* points = pts->data();
	glBufferData(GL_ARRAY_BUFFER, pts->size() * sizeof(float), points, GL_STATIC_DRAW);
	glBindVertexArray(textvao);

	glEnableVertexAttribArray(glGetAttribLocation(textProg, "pos"));
	glBindBuffer(GL_ARRAY_BUFFER, textvbo);
	glVertexAttribPointer(glGetAttribLocation(textProg, "pos"),
		2, GL_FLOAT, GL_FALSE, 4*sizeof(float), NULL);


	
	glEnableVertexAttribArray(glGetAttribLocation(textProg, "texPos"));
	glBindBuffer(GL_ARRAY_BUFFER, textvbo);
	glVertexAttribPointer(glGetAttribLocation(textProg, "texPos"),
		2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (char*)(sizeof(float)*2));

	textScaleX = .2/t->height; //use scale of a single character or you get thin text
	textScaleY = .2/43;
	textureW = al_get_bitmap_width(bitmap3);
	textureH = al_get_bitmap_height(bitmap3);



	//first textPos is 2 float into array
	//printf("glerr %d at %d\n", glGetError(), __LINE__);

	//printf("glerr %d at %d\n", glGetError(), __LINE__);
	textbox = new BaseBox(prog, dimsss, glm::vec4(0, 1, 1, 1));
	texture3 = al_get_opengl_texture(bitmap3);
	textbox->setTexture(texture3, 1, 1);

	float scrolDim[4];
	scrolDim[0] = .5;
	scrolDim[1] = .5;
	scrolDim[2] = 1;
	scrolDim[3] = 1;
	scrollDemo = new scrollable(prog, scrolDim , .1);
	scrollDemo->setRelitivePos(.2, .4);
	float contentDim[4] = { .9,.2,1,1 };
	scrollDemo->addContent(new BaseBox(prog, contentDim, glm::vec4(.3, .3, .3, 1)));
	scrollDemo->addContent(new BaseBox(prog, contentDim, glm::vec4(.6, .3, .3, 1)));
	scrollDemo->addContent(new BaseBox(prog, contentDim, glm::vec4(.3, .6, .3, 1)));
	scrollDemo->addContent(new BaseBox(prog, contentDim, glm::vec4(.3, .3, .6, 1)));
	scrollDemo->addContent(new BaseBox(prog, contentDim, glm::vec4(.3, .6, .3, 1)));
	scrollDemo->addContent(new BaseBox(prog, contentDim, glm::vec4(.3, .3, .6, 1)));
	scrollDemo->addContent(new BaseBox(prog, contentDim, glm::vec4(.3, .6, .3, 1)));
	scrollDemo->addContent(new BaseBox(prog, contentDim, glm::vec4(.3, .3, .6, 1)));
	scrollDemo->addContent(new BaseBox(prog, contentDim, glm::vec4(.3, .6, .3, 1)));
	scrollDemo->addContent(new BaseBox(prog, contentDim, glm::vec4(.3, .3, .6, 1)));

	ALLEGRO_BITMAP* toggleBitmapOff = al_load_bitmap("images/tgboxOff.PNG");
	ALLEGRO_BITMAP* toggleBitmapOn = al_load_bitmap("images/tgboxOn.PNG");
	float toggleDim[4] = { .07,.1,1,1 };
	toggleBox = new toggle(prog, toggleDim, al_get_opengl_texture(toggleBitmapOff), al_get_opengl_texture(toggleBitmapOn));
	toggleBox->setRelitivePos(.93, .9);

	float textBoxDim[4] = { .3,.3,1,1 };
	textBoxTest = new textBox(prog, textBoxDim, glm::vec4(1, 0, 0, 1), textProg, t, texture3, al_get_bitmap_width(bitmap3), al_get_bitmap_height(bitmap3));
	textBoxTest->setTextLineLength(0);
	textBoxTest->setRelitivePos(.3, .3);
	textBoxTest->setText("text in a box?");


	printf("glerr %d at %d\n", glGetError(), __LINE__);
}
int i = 0;
void renderUI2(GLint ui_programme) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(ui_programme);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	box->draw();
}
void renderUI(GLint ui_programme) {
	i++;
	shadeBox->renderAlternate();
	scrollDemo->renderAlternate();
	textBoxTest->renderAlternate();
	//printf("glerr %d at %d\n", glGetError(), __LINE__);

	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(ui_programme);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	
	/*
	//draw in red
	//find fragment uniform "input color"
	GLint fu_inputColour = glGetUniformLocation(ui_programme, "inputColour");
	//set color to red
	glUniform4f(fu_inputColour, i / 180.0, i / (180.0 * 3), i / (180.0 * 4), 1);

	glBindVertexArray(UIBaseVao);
	// draw points 0-3 from the currently bound VAO with current in-use shader
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//printf("\nframe %d", i);/**/
	//printf("glerr %d at %d\n", glGetError(), __LINE__);
	box->draw();
	///textbox->draw();
	///shadeBox->draw();
	//bbox->draw();
	//printf("glerr %d at %d\n", glGetError(), __LINE__);


	///scrollDemo->draw();
	///textBoxTest->draw();
	///toggleBox->draw();

	//
	//printf("glerr %d at %d\n", glGetError(), __LINE__);
	glUseProgram(textProg);
	
	glBindTexture(GL_TEXTURE_2D, texture3);
	
	glUniform2f(glGetUniformLocation(textProg, "textPos"), (frameCount%750)/250.0-1.5, 0.5);

	glUniform2f(glGetUniformLocation(textProg, "textScale"), textScaleX, textScaleY);
	
	glUniform2f(glGetUniformLocation(textProg, "tex_size"), textureW, textureH);

	glUniform3f(glGetUniformLocation(textProg, "textColor"), (frameCount%255) / 255.0, ((frameCount+90)%255) / 255.0, ((frameCount+180)%244)/255.0);

	glBindVertexArray(textvao);
	
	///glDrawArrays(GL_TRIANGLES, 0, pts->size() / 4);
	//printf("glerr %d at %d\n", glGetError(), __LINE__);
	//

}
bool cont = true;
int shakerCount = 0;
float nestNum = 1;
bool alreadyDown = false;
int lastButtons = 0;
void mainLoop(GLint defaultWorldShaders, GLint defaultUiShader, GLint textShaderProgram) {
	while (cont) {
		ALLEGRO_KEYBOARD_STATE key_state;
		al_get_keyboard_state(&key_state);
		if (al_key_down(&key_state,ALLEGRO_KEY_ESCAPE)) {
			cont = false;
		}
		if (al_key_down(&key_state, ALLEGRO_KEY_S)) {
			scrollDemo->scroll(-1);
		}
		if (al_key_down(&key_state, ALLEGRO_KEY_D)) {
			scrollDemo->scroll(1);
		}
		if (al_key_down(&key_state, ALLEGRO_KEY_W)) {
			box->setRelitivePos(.1 + .01*sin(2 * shakerCount++), .05 + .01*cos(shakerCount));
		}
		if (al_key_down(&key_state, ALLEGRO_KEY_SPACE)) {
			if (!alreadyDown) {
				nestNum = nestNum + .3;
				float dims[4] = { .7 / nestNum,.4 / nestNum,1,1 };
				BaseBox* tmp = new BaseBox(defaultUiShader, dims, glm::vec4(nestNum - 1.3, nestNum - 1, nestNum*nestNum - 1, 1));
				deep->setChild(tmp);
				deep = tmp;
				alreadyDown = true;
			}
		}
		else {
			alreadyDown = false;
		}
		ALLEGRO_MOUSE_STATE mouse_state;
		al_get_mouse_state(&mouse_state);
		
		float mx = mouse_state.x;
		float my = screen_height-mouse_state.y;
		if (mouse_state.buttons != 0) {
			box->processUIinput(glm::vec2(mx / screen_width, my / screen_height), mouse_state.buttons, 0, 0);//key and mods are usefull if you want to track things like ctr-click or shift-click
			scrollDemo->processUIinput(glm::vec2(mx / screen_width, my / screen_height), mouse_state.buttons, 0, 0);
			if (mouse_state.buttons != lastButtons) {
				toggleBox->processUIinput(glm::vec2(mx / screen_width, my / screen_height), mouse_state.buttons, 0, 0);
			}
		}
		lastButtons = mouse_state.buttons;
		//al_clear_to_color(al_map_rgba_f(0.0, 0.0, 0.0, 0.0));
		///renderWorld(defaultWorldShaders);
		//printf("glerr %d at %d\n", glGetError(), __LINE__);
		renderUI(defaultUiShader);
		//printf("glerr %d at %d\n", glGetError(), __LINE__);
		glUseProgram(0);

		

		//printf("flipping table\n");
		al_flip_display();
		//printf("table flipped\n");
		al_rest(.015);
	}
}

/*void AltTest(int shader_programme){
	//next http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-9-vbo-indexing/

	float points[] = {
		0.0f,  0.5f,  0.0f,
		0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  -0.5f
	};


	GLuint vbo = 0;
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (float), points, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	for(int i = 0;i<1800;i++){
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram (shader_programme);

		//draw in red
		//find fragment uniform "input color"
		GLint fu_inputColour = glGetUniformLocation(shader_programme, "inputColour");
		//set color to red
		glUniform4f(fu_inputColour, i/180.0,i/(180.0*3),i/(180.0*4),1);

		glBindVertexArray (vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays (GL_TRIANGLES, 0, 3);
		al_flip_display();
		printf("\nframe %d",i);
		al_rest(.013);
	}
}/**/
/*
int testShaderErr(GLuint program){
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		for(int i=0;i<infoLog.size();i++){
			putchar(infoLog[i]);
		}

		//The program is useless now. So delete it.
		glDeleteProgram(program);

		//Provide the infolog in whatever manner you deem best.
		//Exit with failure.
		return 1;
	}
	return 0;
}*/

int main(int argc, char **argv)
{

	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	if (!al_init_image_addon()) {
		printf("image or font ded\n");
		return -1;
	}
	if (!al_init_font_addon()) {
		printf("font ded\n");
		return -1;
	}
	
	if (!al_init_ttf_addon()) {
		printf("font ded more\n");
		return -1;
	}

	display = createOpenGlDisplay();

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize allegro keyboard!\n");
		return -1;
	}
	if (!al_install_mouse()) {
		fprintf(stderr, "failed to initialize allegro mouse!\n");
		return -1;
	}
	if(!display){
		fprintf(stderr, "failed to initialize oppenGl display!\n");
		return -1;
	}

	
	if (FT_Init_FreeType(&library))
	{
		fprintf(stderr, "failed to initialize TTF!\n");
		return -1;
	}

	int error = FT_New_Face(library,
		"text/ostrich-regular.ttf",
		0,
		&face);
	if (error == FT_Err_Unknown_File_Format)
	{
		fprintf(stderr, "failed to read font format, file found!\n");
		return -2;
	}
	else if (error)
	{
		fprintf(stderr, "failed to read font format, file not found!\n");
		return -3;
	}

	error = FT_Set_Pixel_Sizes(
		face,   /* handle to face object */
		0,      /* pixel_width           */
		16);   /* pixel_height          */

	al_clear_to_color(al_map_rgb(frameCount*10%255,0,0));
	al_flip_display();

	//compile shaders
	GLuint vert,frag;
	compileBasicShaders(&vert,&frag);

	GLuint pVert, pFrag;
	pVert = compileShader("./shaders/simple_vert.glsl", GL_VERTEX_SHADER);
	pFrag = compileShader("./shaders/simple_frag.glsl", GL_FRAGMENT_SHADER);
	//testShaderErr(pVert);
	//testShaderErr(pFrag);
	//glGetError();
	GLuint phongProg = linkShader(pVert, pFrag);
	testProgErr(phongProg);
	printf("der\n");

	GLuint UiVert, UiFrag;
	UiVert = compileShader("./shaders/box_vert.glsl", GL_VERTEX_SHADER);
	UiFrag = compileShader("./shaders/box_frag.glsl", GL_FRAGMENT_SHADER);
	testShaderErr(UiVert);
	testShaderErr(UiFrag);
	//glGetError();
	GLuint UiProg = linkShader(UiVert, UiFrag);
	testProgErr(UiProg);
	printf("glerr %d at %d\n", glGetError(), __LINE__);
	printf("der\n");
	
	//link shader program
	GLuint shader_programme = glCreateProgram ();
	glAttachShader (shader_programme, frag);
	glAttachShader (shader_programme, vert);

	//testShaderErr(frag);
	//testShaderErr(vert);

	glLinkProgram (shader_programme);



	//////////////////////////////////////////////////////
	GLuint textVert, textFrag;
	textVert = compileShader("./shaders/text_vert.glsl", GL_VERTEX_SHADER);
	textFrag = compileShader("./shaders/text_frag.glsl", GL_FRAGMENT_SHADER);
	testShaderErr(textVert);
	testShaderErr(textFrag);
	//glGetError();
	textProg = linkShader(textVert, textFrag);
	testProgErr(textProg);
	printf("glerr %d at %d\n", glGetError(), __LINE__);
	printf("der\n");
	//////////////////////////////////////////////////////


	

	printf("\n");
	if(testProgErr(shader_programme)){
		char a;
		scanf("%c",&a);
		return -1;
	}
	t = new TextTex("test/anonymous_regular_65.lua");
	printf("len is %d\n", t->texWidth("hello World!"));
	printf("height is %d\n", t->texHeight(88, "hello World!"));
	printf("tri Time\n");
//	pts = t->TextToTex("Hello World!");

	initCamera();
	//AltTest(shader_programme);
	initWorldGround(phongProg);
	initUIBase(UiProg);

	altShadeTestTmp = phongProg;

	mainLoop(phongProg, UiProg, textProg);
	al_destroy_display(display);

	return 0;
}