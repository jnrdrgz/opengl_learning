#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

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

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(5);

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR\n";
	}
	std::cout << glGetString(GL_VERSION) << "\n";
	std::cout << "test\n";


	float positions[] = {
		-0.5f, -0.5f, // 0
		 0.5f, -0.5f, // 1
		 0.5f,	0.5f, // 2
		-0.5f,	0.5f  // 3
	};

	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	//BIND == SELECT
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

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

	glBindVertexArray(0);
	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	float r = 0.0f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//glDrawArrays(GL_TRIANGLES, 0, 3);

		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, .5f));

		//gone thanks to the vao
		//GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);


		GLCall(glBindVertexArray(vao));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));


		if (r > 1.0f) {
			r = 0.0f;
		}
		else {
			r += 0.05;
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}