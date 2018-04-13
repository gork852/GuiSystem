// ttf to Lua table (3rd setting) and png via: https://github.com/andryblack/fontbuilder/downloads

//going to need to include this if you dont
#include <vector>

//some variables i used

TextTex* t = NULL;
float textScaleX, textScaleY;
float textureW, textureH;
GLuint textvbo;
GLuint textvao;
std::vector<float>* pts;
GLuint textProg;
GLint texture3; //going to have to figure out how to load a png file
int frameCount; // only used to set color and make text move


//init

t = new TextTex("test/anonymous_regular_65.lua");//use file path to lua file

char* testText = "hello world!";
pts = t->TextToTex(testText);
glGenVertexArrays(1, &textvao);
glGenBuffers(1, &textvbo);
glBindBuffer(GL_ARRAY_BUFFER, textvbo);
float* points = pts->data();
glBufferData(GL_ARRAY_BUFFER, pts->size() * sizeof(float), points, GL_STATIC_DRAW);
glBindVertexArray(textvao);

glEnableVertexAttribArray(glGetAttribLocation(textProg, "pos"));
glBindBuffer(GL_ARRAY_BUFFER, textvbo);
glVertexAttribPointer(glGetAttribLocation(textProg, "pos"), 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), NULL);

glEnableVertexAttribArray(glGetAttribLocation(textProg, "texPos"));
glBindBuffer(GL_ARRAY_BUFFER, textvbo);
glVertexAttribPointer(glGetAttribLocation(textProg, "texPos"), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (char*)(sizeof(float)*2));

textScaleX = .2/t->height; 
textScaleY = .2/43; //use scale of a single character or you get thin text
textureW = al_get_bitmap_width(bitmap3);
textureH = al_get_bitmap_height(bitmap3);


//draw text last
glClear(GL_DEPTH_BUFFER_BIT);
glUseProgram(textProg);

glBindTexture(GL_TEXTURE_2D, texture3);
glUniform2f(glGetUniformLocation(textProg, "textPos"), (frameCount%750)/250.0-1.5, 0.5);
glUniform2f(glGetUniformLocation(textProg, "textScale"), textScaleX, textScaleY);
glUniform2f(glGetUniformLocation(textProg, "tex_size"), textureW, textureH);
glUniform3f(glGetUniformLocation(textProg, "textColor"), (frameCount%255) / 255.0, ((frameCount+90)%255) / 255.0, ((frameCount+180)%244)/255.0);
glBindVertexArray(textvao);
glDrawArrays(GL_TRIANGLES, 0, pts->size() / 4);