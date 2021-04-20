#include <GL/glew.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <SDL.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* fun, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::cout << "[opengl error] (" << error << ")" 
			<< " file:" << file << " function" << fun 
			<< " line:" << line << "\n";
		return false;
	}

	return true;
}


const int screen_w = 640;
const int screen_h = 480;

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(std::string filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	auto find_in_line = [](const std::string& l, const std::string& v) {return l.find(v) != std::string::npos; };

	while (getline(stream, line)) {
		if (find_in_line(line, "#shader"))
		{
			if (find_in_line(line, "vertex")) type = ShaderType::VERTEX;
			else if (find_in_line(line, "fragment")) type = ShaderType::FRAGMENT;
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[0].str(),ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	//when pass pointer src you are passing a pointer to pointer
	//(an array of array), so you pass the pointer of the pointer to src
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//TODO error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " 
			<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "\n";
		std::cout << message << "\n";
	}


	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	return program;
}

int main(int argc, char* args[])
{

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Test", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 
		screen_w,screen_h,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR\n";
	}
	std::cout << glGetString(GL_VERSION) << "\n";
	std::cout << "test\n";
	SDL_Log("\nGL_VERSION   : %s\nGL_VENDOR    : %s\nGL_RENDERER  : %s\n"
		"GLEW_VERSION : %s\nGLSL VERSION : %s\n",
		glGetString(GL_VERSION), glGetString(GL_VENDOR),
		glGetString(GL_RENDERER), glewGetString(GLEW_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));
	int maj;
	int min;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &maj);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &min);
	SDL_Log("Using OpenGL %d.%d", maj, min);

	unsigned int VBO;
	unsigned int quadVAO;
	float vertices[] = {
		// pos      
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};

	GLCall(glGenVertexArrays(1, &quadVAO));
	GLCall(glGenBuffers(1, &VBO));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
	GLCall(glBindVertexArray(quadVAO));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));

	//std::string vertexShader 
		
	//std::string fragmentShader

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	std::cout << "Vertex:\n";
	std::cout << source.VertexSource << "\n";
	std::cout << "Fragment:\n";
	std::cout << source.FragmentSource << "\n";

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	int location = glGetUniformLocation(shader, "u_Color");
	if (location != -1) {
		
	}

	bool running = true;
	SDL_Event event;
	glViewport(0, 0, screen_w, screen_h);
	GLCall(glUniform4f(location, 1.0f, 0.3f, 0.8f, 1.0f));

	/* Loop until the user closes the window */
	while (running)
	{
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				running = 0;
		}

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
	
		GLCall(glBindVertexArray(quadVAO));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		GLCall(glBindVertexArray(0));
		
		SDL_GL_SwapWindow(window);
		/* Poll for and process events */
		//glfwPollEvents();
	}

	glDeleteProgram(shader);

	//glfwTerminate();
	return 0;
}