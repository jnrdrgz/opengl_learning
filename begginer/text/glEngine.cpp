#include "glEngine.h"

glEngine::glEngine() {
	fontSize = 20;
	fontSpace = 15;
	fontBase = 0;
	fontTexture = nullptr;

	printf("glEngine constructed\n");
}
glEngine::~glEngine() {

}

GLvoid glEngine::uninitialize(GLvoid){
	delete glEngine::getEngine();
}

GLvoid glEngine::initialize(GLint widht, GLint height) {
	Light::initialize();

	fontTexture = new Texture("font.tga", "Grass Surface Texture");

	buildTextureFont();
}

glEngine* glEngine::getEngine(GLvoid){
	static glEngine* engine = new glEngine();

	return engine;
}

GLvoid glEngine::buildTextureFont(GLvoid) {
	fontBase = glGenLists(256);
	glBindTexture(GL_TEXTURE_2D, fontTexture->textID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC0_ALPHA, GL_ONE);

	float len_char_t = 1.0f / 16.0f;

	for (int i = 0; i < 256; i++) {
		float cx = (float)(i % 16) / 16.0f;
		float cy = (float)(-i / 16) / 16.0f;

		glNewList(fontBase + i, GL_COMPILE);

		glBegin(GL_QUADS);

			/*glTexCoord2f(cx,			1 - cy - len_char_t); glVertex2i(0, fontSize);
			glTexCoord2f(cx+len_char_t, 1 - cy - len_char_t); glVertex2i(fontSize, fontSize);
			glTexCoord2f(cx+len_char_t, 1 - cy);			  glVertex2i(fontSize, 0);
			glTexCoord2f(cx,			1 - cy);			  glVertex2i(0, 0);
			*/

		glTexCoord2f(cx, 1 - cy);			glVertex2i(0, fontSize);
		glTexCoord2f(cx + len_char_t, 1 - cy);			glVertex2i(fontSize, fontSize);
		glTexCoord2f(cx + len_char_t, 1 - cy - len_char_t);	glVertex2i(fontSize, 0);
		glTexCoord2f(cx, 1 - cy - len_char_t);	glVertex2i(0, 0);

		glEnd();

		glTranslated(fontSpace, 0, 0);

		glEndList();
	}
}

GLvoid glEngine::drawText(GLint x, GLint y, const char* in_text, ...) {

	char text[256];

	va_list ap;
	va_start(ap, in_text);
		vsprintf_s(text, in_text, ap);
	va_end(ap);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC0_ALPHA, GL_ONE);

	glBindTexture(GL_TEXTURE_2D, fontTexture->textID);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadIdentity();

	glTranslated(x,y,0);
	glListBase(fontBase - 16);
	glCallLists(strlen(text), GL_BYTE, text);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

GLuint glEngine::getTextWidth(const char* text) {
	return (strlen(text) * fontSpace);

}

GLuint glEngine::getTextHeight(const char* text)
{
	return fontSize;
}
