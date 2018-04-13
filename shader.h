

GLuint compileShader(char*,GLenum);
void compileBasicShaders(GLuint*,GLuint*);
GLuint linkShader(GLuint, GLuint);
int testShaderErr(GLuint);
int testProgErr(GLuint);