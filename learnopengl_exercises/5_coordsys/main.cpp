#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "glm/gtc/type_ptr.hpp"

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

struct Shader 
{
	Shader(std::string filepath) {
		ShaderProgramSource source = ParseShader(filepath);

		std::cout << "Shader Created\n";
		std::cout << "Vertex:\n";
		std::cout << source.VertexSource << "\n";
		std::cout << "Fragment:\n";
		std::cout << source.FragmentSource << "\n";

		shader_id = CreateShader(source.VertexSource, source.FragmentSource);
	}
	
	void use() {
		GLCall(glUseProgram(shader_id));
	}

	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(shader_id, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(shader_id, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(shader_id, name.c_str()), value);
	}

	unsigned int shader_id;
};

unsigned int create_vbo_vao_ebo(float* vertices, GLsizeiptr vertices_size) {
	unsigned int VBO, VAO, EBO;

	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));
	
	GLCall(glBindVertexArray(VAO));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW));
		
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));

	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(2));

	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));

	return VAO;
}



void draw(unsigned int vao) {
	GLCall(glBindVertexArray(vao));

	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		std::cout << "up\n";
	}
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
	//glfwSwapInterval(5);

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR\n";
	}
	std::cout << glGetString(GL_VERSION) << "\n";
	std::cout << "test\n";

	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 1.0f));
	vec = trans * vec;
	std::cout << vec.x << "-" << vec.y << "-" << vec.z << "\n";

	
	glEnable(GL_DEPTH_TEST);
	
	ShaderProgramSource source = ParseShader("res/shaders/text.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	//TEXTURE
	unsigned int texture;
	GLCall(glGenTextures(1, &texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int t_width, t_height, nrChannels;
	//
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("container.jpg", &t_width, &t_height, &nrChannels, 0);
	if (data) {
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "error loading texture\n";
	}

	stbi_image_free(data);


	//texture2
	unsigned int texture2;
	GLCall(glGenTextures(1, &texture2));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture2));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data2 = stbi_load("awesomeface.png", &t_width, &t_height, &nrChannels, 0);
	if (data) {
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "error loading texture\n";
	}

	stbi_image_free(data2);



	//std::cout << "Vertex:\n";
	//std::cout << source.VertexSource << "\n";
	//std::cout << "Fragment:\n";
	//std::cout << source.FragmentSource << "\n";
	//
	
	GLCall(glUseProgram(shader));
	GLCall(glUniform1i(glGetUniformLocation(shader, "texture1"), 0));
	GLCall(glUniform1i(glGetUniformLocation(shader, "texture2"), 1));

	float r = 0.0f;
	float tt = 0.1;
	glUniform1f(glGetUniformLocation(shader, "u_text_trans"), tt);
	/* Loop until the user closes the window */

	unsigned int projection_loc = glGetUniformLocation(shader, "projection");
	unsigned int view_loc = glGetUniformLocation(shader, "view");
	unsigned int model_loc = glGetUniformLocation(shader, "model");
	//glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	float near_pr = 0.1f;
	float far_pr = 100.0f;
	float camera_z_rot = -3.0f;


	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			if (tt < 1.0) {
				tt += 0.01;
				std::cout << tt << "\n";
			}

			unsigned int tt_location = glGetUniformLocation(shader, "u_text_trans");
			glUniform1f(tt_location, tt);
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			if (tt > 0.0) {
				tt -= 0.01;
				std::cout << tt << "\n";
			}

			unsigned int tt_location = glGetUniformLocation(shader, "u_text_trans");
			glUniform1f(tt_location, tt);
		}


		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			near_pr += 0.01f;
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
			near_pr -= 0.01f;
		}

		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
			far_pr += 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
			far_pr -= 0.1f;
		}


		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camera_z_rot += .01f;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			camera_z_rot -= 0.01f;
		}


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glDrawArrays(GL_TRIANGLES, 0, 3);

		float timeValue = glfwGetTime();
		float rValue = sin(timeValue) / 2.0f + 0.5f;

		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture));
		//aca otra texture si es que hay
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture2));
	
		GLCall(glUseProgram(shader));

		//glm::mat4 model = glm::mat4(1.0f);
		//glm::mat4 view = glm::mat4(1.0f);
		//glm::mat4 projection = glm::mat4(1.0f);

		// create transformations
		glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, near_pr, far_pr);
		view = glm::translate(view, glm::vec3(sin(glfwGetTime()), 0.0f, camera_z_rot));

		glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

		GLCall(glBindVertexArray(VAO));

		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 _model = glm::mat4(1.0f);
			_model = glm::translate(_model, cubePositions[i]);
			float angle = 20.0f * i;
			_model = glm::rotate(_model, (float)glfwGetTime()*glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(_model));

			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36););
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
