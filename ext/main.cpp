#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

//glm
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
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

unsigned int SCREEN_W = 860;
unsigned int SCREEN_H = 640;


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

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

void processInput(GLFWwindow* window);

struct SpaceShip
{
	void load(glm::vec3 position, unsigned int shader) {
		this->shaderId = shader;
		model_location = glGetUniformLocation(shaderId, "model");

		this->position = position;

		GLCall(glGenVertexArrays(1, &VAO));
		GLCall(glGenBuffers(1, &VBO));

		GLCall(glBindVertexArray(VAO));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
		GLCall(glEnableVertexAttribArray(0));
	}

	void draw() {
		GLCall(glUseProgram(shaderId));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::translate(model, position * glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

		GLCall(glBindVertexArray(VAO));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3););
	}

	void handle_input(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			position.x += 0.01;
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
			position.x -= 0.01;
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			position.y += 0.01;
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
			position.y -= 0.01;
	}

	float vertices[9] = {
	   0.5f, -0.5f, 0.0f,
	  -0.5f, -0.5f, 0.0f,
	   0.0f,  0.5f, 0.0f
	};

	unsigned int shaderId;
	unsigned int VBO, VAO;

	glm::vec3 position;

	//move to shader class
	unsigned int model_location;

};

struct Camera
{
	Camera(int shader) {
		this->shaderId = shader;

		projection_location = glGetUniformLocation(shaderId, "projection");
		view_location = glGetUniformLocation(shaderId, "view");
		model_location = glGetUniformLocation(shaderId, "model");

		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_W / (float)SCREEN_H, 0.1f, 100.0f);
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
	}

	void update() {
		GLCall(glUseProgram(shaderId));
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
	}

	void handle_input(GLFWwindow* window) {
		float cameraSpeed = 0.01;// *deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {

		}


	}

	int shaderId;
	unsigned int projection_location;
	unsigned int view_location;
	unsigned int model_location;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;


};


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

struct m_Camera {
	m_Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
		float yaw = YAW, 
		float pitch = PITCH) 
		: 
		front(glm::vec3(0.0f, 0.0f, -1.0f)), 
		movementSpeed(SPEED), 
		mouseSens(SENSITIVITY), 
		zoom(ZOOM)
	{
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;

		
		updateCameraVectors();
	}
	
	m_Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
		: 
		front(glm::vec3(0.0f, 0.0f, -1.0f)),
		movementSpeed(SPEED),
		mouseSens(SENSITIVITY),
		zoom(ZOOM)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();
	}

	void update_locations() {
		projection_location = glGetUniformLocation(shaderId, "projection");
		view_location = glGetUniformLocation(shaderId, "view");
	}

	glm::mat4 getMatrixView()
	{
		return glm::lookAt(position, position + front, up);
	}

	void updateCameraVectors() {
		//scope for new front
		{
			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

			this->front = glm::normalize(front);
		}

		right = glm::normalize(glm::cross(front, up));
		up = glm::normalize(glm::cross(right, front));
	}

	void update() {
		GLCall(glUseProgram(shaderId));

		glm::mat4 projection = glm::perspective(glm::radians(zoom), (float)SCREEN_W / (float)SCREEN_H, 0.1f, 100.0f);
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

		// camera/view transformation
		glm::mat4 view = getMatrixView();
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

		
	}

	glm::vec3 position, front, up, right, worldUp;
	float yaw, pitch, movementSpeed, mouseSens, zoom;
	unsigned int shaderId, projection_location, view_location, model_location;
	
};

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
	window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	//glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR\n";
	}
	std::cout << glGetString(GL_VERSION) << "\n";
	std::cout << "test\n";


	float vertices[] = {
	   0.5f, -0.5f, 0.0f,  // bottom right
	  -0.5f, -0.5f, 0.0f,  // bottom left
	   0.0f,  0.5f, 0.0f   // top 
	};

	//unsigned int indices[] = {
	//	0,1,2,
	//	2,3,0
	//};

	unsigned int VBO, VAO;

	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));

	GLCall(glBindVertexArray(VAO));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
	GLCall(glEnableVertexAttribArray(0));

	//unsigned int ibo;
	//glGenBuffers(1, &ibo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glm::vec3 v3 = glm::vec3(0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	std::cout << "Vertex:\n";
	std::cout << source.VertexSource << "\n";
	std::cout << "Fragment:\n";
	std::cout << source.FragmentSource << "\n";

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	//glm::mat4 view = glm::mat4(1.0f); 
	//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	//glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	//Camera camera(shader);
	m_Camera m_camera(glm::vec3(0.0f, 0.0f, 3.0f));
	m_camera.shaderId = shader;
	m_camera.update_locations();

	SpaceShip ship;
	SpaceShip ship2;

	ship.load(glm::vec3{ 0.5f, -0.5f, 0.0f }, shader);
	ship2.load(glm::vec3{ -0.5f, 0.5f, 0.0f }, shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		processInput(window);
		//camera.handle_input(window);

		GLCall(glUseProgram(shader));

		ship.handle_input(window);

		m_camera.update();
		ship.draw();
		ship2.draw();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}