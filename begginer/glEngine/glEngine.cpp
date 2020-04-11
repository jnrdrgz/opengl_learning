#include "glEngine.h"

glEngine::glEngine() {

}
glEngine::~glEngine() {

}

GLvoid glEngine::uninitialize(GLvoid){
	delete glEngine::getEngine();
}

GLvoid glEngine::initialize(GLint widht, GLint height) {
	Light::initialize();
}

glEngine* glEngine::getEngine(GLvoid){
	static glEngine* engine = new glEngine();

	return engine;
}