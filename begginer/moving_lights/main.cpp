#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include "texture.h"
#include "main.h"
#include "light.h"

double get_rand_double() {
	return (double)(rand() % 10000);
}

const GLsizei windowW = 500;
const GLsizei windowH = 500;
SDL_Window* window = nullptr;

GLfloat cubeRotateX = 45.0f;
GLfloat cubeRotateY = 45.0f;

Uint8 *keys = nullptr;

//Texture* texture = nullptr;
Light* mainLight = nullptr;


Uint32* seed = NULL;
std::vector<double> nums_for_lights = { get_rand_double(), get_rand_double(), get_rand_double(), get_rand_double(), 
											get_rand_double(), get_rand_double(), get_rand_double(), get_rand_double() };


//INIT OPEN GL
GLvoid establishProjectionMatrix(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 200.0f);
}

GLvoid initGL(GLsizei width, GLsizei height) {
	Light::initialize();

	establishProjectionMatrix(width, height);

	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);
	//glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);


	//texture = new Texture("grass.tga", "Grass Surface Texture");
}

GLvoid displayFPS(GLvoid) {
	static long lastTime = SDL_GetTicks();
	static long loops = 0;
	static GLfloat fps = 0.0f;

	int newTime = SDL_GetTicks();

	if (newTime - lastTime > 100) {
		float newFPS = (float)loops / float(newTime - lastTime) * 1000.0f;

		fps = (fps + newFPS) / 2.0f;

		char title[80];
		sprintf_s(title, "OpenGL - DEMO %f", fps);

		
		SDL_SetWindowTitle(window, title);

		lastTime = newTime;
		loops = 0;
	}

	loops++;
}

GLvoid draw_cube(GLvoid) {

	const float width = 80.0f;
	const float height = 80.0f;

	const int divisions = 100;

	float incX = width / (float)divisions;
	float incY = height / (float)divisions;

	glColor3f(0,0,0);
	glNormal3f(0,1,0);

	for (float x = -width / 2; x < width / 2; x += incX) {
		for (float y = -height / 2; y < height / 2; y += incY) {
			glBegin(GL_TRIANGLE_STRIP);

				glVertex3f(x + incX, 0.0f, y + incY);
				glVertex3f(x, 0.0f,	y + incY);
				glVertex3f(x + incX, 0.0f, y);
				glVertex3f(x	   , 0.0f, y);

			glEnd();
			

		}
	}
	
}

GLvoid drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -120.0f);
	glRotatef(cubeRotateX, 1, 0, 0);
	glRotatef(cubeRotateY, 0, 1, 0);

	int seed_ind = 0;
	for (auto l : Light::lights) {
		double randomNumber = (double)SDL_GetTicks()+nums_for_lights[seed_ind];
		float x = (float)sin(randomNumber / 1600.0f)
			* (float)cos(randomNumber / 1600.0f) * 50.0f;

		float y = (float)sin(randomNumber / 900.0f)
			* (float)cos(randomNumber / 1400.0f) * 50.0f;

		l->setPosition(x, 30.0f, y);


		l->updateLight();
		seed_ind++;
	}

	//glBindTexture(GL_TEXTURE_2D, texture->textID);

	draw_cube();

	glFlush();

	SDL_GL_SwapWindow(window);

	displayFPS();
}



//keys_handling
GLfloat rotation_speed = 1.0f;
void handleKeys(unsigned char key, int x, int y) {

	//const GLfloat speed = 1.0f;

	if (key == 'q')
		cubeRotateY -= rotation_speed;
	if (key == 'w')
		cubeRotateX -= rotation_speed;

	/*
	if (keys[SDLK_ESCAPE])
		return true;

	if (keys[SDLK_LEFT])
		cubeRotateY -= speed;

	if (keys[SDLK_RIGHT])
		cubeRotateY += speed;

	if (keys[SDLK_UP])
		cubeRotateX -= speed;

	if (keys[SDLK_DOWN])
		cubeRotateX += speed;

	*/
}

void handle_key_down_test(SDL_Keycode key_code) {	
	if (key_code == SDLK_LEFT)
		cubeRotateY -= rotation_speed;

	if (key_code == SDLK_RIGHT)
		cubeRotateY += rotation_speed;

	if (key_code == SDLK_UP)
		cubeRotateX -= rotation_speed;

	if (key_code == SDLK_DOWN)
		cubeRotateX += rotation_speed;

	if (key_code == SDLK_PLUS)
		rotation_speed++;

	if (key_code == SDLK_MINUS)
		rotation_speed--;

}

int main(int argc, char* args[])
{
	const int screenWidth = 500;
	const int screenHeight = 500;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	GLuint a = 2;

	window = SDL_CreateWindow("SDL OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	SDL_GLContext myopengl_context = SDL_GL_CreateContext(window);

	if (myopengl_context == NULL) printf("Error creating OpenGL context, SDL Error: %s\n", SDL_GetError());
	if (SDL_GL_SetSwapInterval(1) < 0) printf("Error setting vsync, SDL_error: %s\n", SDL_GetError());

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	initGL(windowW, windowH);

	for (int i = 0; i < Light::numLights; i++) {
		Light* light = new Light(LIGHT_SPOT);

		float r = (float)rand() / (float)RAND_MAX;
		float g = (float)rand() / (float)RAND_MAX;
		float b = (float)rand() / (float)RAND_MAX;

		light->setDiffuse(r, g, b, 1);
		light->setAmbient(0, 0, 0, 1);
		light->setSpotDirection(0, -1, 0);

		float cutoff = 20.0f + (float)(rand() % 60);

		light->setCutoff(cutoff);

		light->setExponent(20.0f*cutoff);
	}

	bool running = true;
	SDL_Event event;
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
			else if (event.type == SDL_TEXTINPUT) {
				int x = 0, y = 0;
				SDL_GetMouseState(&x, &y);
				handleKeys(event.text.text[0], x, y);
			}
			else if (event.type == SDL_KEYDOWN) {
				handle_key_down_test(event.key.keysym.sym);
			}
		}

		drawScene();

	}

	//for (auto l : Light::lights) {
	//	delete l;
	//}

	//delete seed;

	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
	return 0;
}