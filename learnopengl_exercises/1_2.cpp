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

unsigned int create_vao(float* vertices, GLsizeiptr vertices_size) {
	unsigned int VBO, VAO;

	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));

	GLCall(glBindVertexArray(VAO));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));

	return VAO;
}

void draw(unsigned int vao) {
	GLCall(glBindVertexArray(vao));

	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
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


	float vertices_t1[] = {
	   -0.5f, -0.5f, 0.0f, // left  
		0.0f, -0.5f, 0.0f, // right 
		-0.25f,  0.25f, 0.0f, // top
	};

	float vertices_t2[] = {
		0.0f, -0.5f, 0.0f, // left
		0.0f, 0.5f, 0.0f, // right  
		0.25f,  0.25f, 0.0f, // top
	};

	unsigned int vao1 = create_vao(vertices_t1, sizeof(vertices_t1));
	unsigned int vao2 = create_vao(vertices_t2, sizeof(vertices_t2));

	//vertex buffer obj, vertex array obj
	


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

	float r = 0.0f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//glDrawArrays(GL_TRIANGLES, 0, 3);

		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, .5f));

		draw(vao1);
		draw(vao2);

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