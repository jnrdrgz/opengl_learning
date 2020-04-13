#include "glEngine.h"
#include "main.h"

#include "button.h"
#include "listBox.h"
#include "slider.h"

const GLsizei windowW = 500;
const GLsizei windowH = 500;
SDL_Window* window = nullptr;

GLfloat rotateX = 45.0f;
GLfloat rotateY = 45.0f;
GLfloat zoom = -5.0f;

Uint8* keys = nullptr;

Texture* texture = nullptr;
Light* mainLight = nullptr;

Control* controlled = nullptr;
MouseState mouseState;

float testValue = 5.0f;

bool navigating = false;

//INIT OPEN GL
GLvoid establishProjectionMatrix(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 200.0f);
}

GLvoid setOrtho(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0,width,height,0);
}


GLvoid initGL(GLsizei width, GLsizei height) {
	glEngine::getEngine()->initialize(width, height);

	establishProjectionMatrix(width, height);

	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	mainLight = new Light(LIGHT_SPOT);
	mainLight->setDiffuse(2.0f, 2.0f, 2.0f, 1.0f);
	mainLight->setPosition(0.0f, 5.0f, 0.0f);

	texture = new Texture("grass.tga", "Grass Surface Texture");

	addControl(new Button("RUN AWAY", 0, 0, 200, 50));
	ListBox* lstNames = (ListBox*)addControl(new ListBox(0, 0, 200, 200));
	lstNames->addItem("ITEM1");
	lstNames->addItem("ITEM2");
	lstNames->addItem("ITEM3");
	lstNames->addItem("ITEM4");

	Slider* testSlider = (Slider*)addControl(new Slider("Test Value", -5.0f, 5.0f, 0, 0, 400, 40));

	testSlider->setValue(&testValue);

}

GLvoid displayFPS(GLvoid) {
	static long lastTime = SDL_GetTicks();
	static long loops = 0;
	static GLfloat fps = 0.0f;

	int newTime = SDL_GetTicks();

	if (newTime - lastTime > 100) {
		float newFPS = (float)loops / float(newTime - lastTime) * 1000.0f;

		fps = (fps + newFPS) / 2.0f;

		/*char title[80];
		sprintf_s(title, "OpenGL - DEMO %f", fps);
		SDL_SetWindowTitle(window, title);*/



		lastTime = newTime;
		loops = 0;
	}

	glEngine::getEngine()->drawText(5,5, "OpenGL - DEMO %f", fps);

	loops++;
}

GLvoid draw_cube() {
	//draw cube

	glBegin(GL_QUADS);


	//TOP FACE
	glNormal3f(0, 1, 0);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);

	//BOTTOM FACE
	glNormal3f(0, -1, 0);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	//FRONT FACE
	glNormal3f(0, 0, -1);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	//BACK FACE
	glNormal3f(0, 0, 1);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);

	//LEFT FACE
	glNormal3f(1, 0, 0);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

	//RIGHT FACE
	glNormal3f(-1, 0, 0);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	glEnd();
}

GLvoid drawControls(GLvoid) {
	for (auto c : Control::controls) {
		c->drawControl();

		if ((controlled != nullptr && controlled != c) || navigating) continue;
		if (c->updateControl(mouseState)) {
			printf("controlled\n");
			controlled = c;

			//handle events
		}
		else if(c == controlled){
			controlled = nullptr;
		}
		else {
			//control has been updated but no messages
		}
	}

}

GLvoid drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//PASS 1
	establishProjectionMatrix(windowW, windowH);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_BLEND);
	glTranslatef(0, 0, zoom);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);

	for (auto l : Light::lights) {
		l->updateLight();

	}

	glColor3f(1.0f, 1.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, texture->textID);

	draw_cube();

	//PASS 2
	glDisable(GL_LIGHTING);
	setOrtho(windowW, windowH);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	displayFPS();
	drawControls();
	

	glFlush();

	SDL_GL_SwapWindow(window);

}



//keys_handling
GLfloat rotation_speed = 1.0f;
void handleKeys(unsigned char key, int x, int y) {

	//const GLfloat speed = 1.0f;

	
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
	
	if (key_code == SDLK_PLUS)
		rotation_speed++;

	if (key_code == SDLK_MINUS)
		rotation_speed--;

}

const float rotateX_speed = 0.5f;
const float rotateY_speed = 0.5f;
const float zoom_speed = 0.01f;
GLboolean updateNavigation(GLvoid) {
	static int lastX = -1;
	static int lastY = -1;

	if (lastX == -1 && lastY == -1) {
		lastX = mouseState.x;
		lastY = mouseState.y;
	}

	int changeX = lastX - mouseState.x;
	int changeY = lastY - mouseState.y;

	lastX = mouseState.x;
	lastY = mouseState.y;

	if (mouseState.LeftButtonDown && controlled == NULL) {
		//SDL_WM_GrabInput(SDL_GRAB_ON);

		rotateX -= (float)changeY * rotateX_speed;
		rotateY -= (float)changeX * rotateY_speed;

		navigating = true;

		printf("in left\n");

	}

	else if (mouseState.RightButtonDown && controlled == NULL) {
		//SDL_WM_GrabInput(SDL_GRAB_ON);

		printf("in right\n");

		zoom -= (float)changeX * zoom_speed;
		navigating = true;
	}
	else {
		//SDL_WM_GrabInput(SDL_GRAB_OFF);

		navigating = false;
	}

	return false;
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

	bool running = true;
	SDL_Event event;
	while (running) {
		SDL_GetMouseState(&mouseState.x, &mouseState.y);
		mouseState.LeftButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1);
		mouseState.MiddleButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(2);
		mouseState.RightButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3);
		
		
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
			else if (event.type == SDL_MOUSEBUTTONDOWN) {

			}
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
			}
			else if (event.type == SDL_MOUSEMOTION) {
			}
			
		}

		drawScene();

		updateNavigation();


	}
	for (auto c : Control::controls) {
		//delete c;
	}
	glEngine::uninitialize();

	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
	return 0;
}