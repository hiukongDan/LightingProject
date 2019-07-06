#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>

#include <iostream>
#include <experimental/filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <nanogui/nanogui.h>

#include "Camera.h"
#include "shader.h"
#include "Model.h"


using namespace Assimp;
using namespace nanogui;
namespace fs = std::experimental::filesystem;

Camera camera;

double lastFrame = 0;
double deltaTime = 0;

bool firstMouse = true;

double xPos, yPos;


enum LightingModel {
	Phong = 0,
	Blinn,
	Gauroud
};

LightingModel enumval = Phong;
Color colval(1.0f, 1.0f, 1.0f,1.0f);

glm::vec3 lightColor = glm::vec3(colval.r(), colval.g(), colval.b());

Screen* screen = nullptr;


void framebuffer_resize_callback(GLFWwindow* window, int x, int y);
void UpdateMousePosition(double xpos, double ypos);
void mouse_left_move_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_right_move_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_middle_pressed_move_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_middle_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void configShader(Shader& shader);

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(900, 600, "Demo", NULL, NULL);

	if (!window) {
		std::cout << "Init WINDOW fail" << std::endl;
		return 1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout<<"Load glloader fail"<<std::endl;
		return 2;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
	glfwSetScrollCallback(window, mouse_middle_scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);


	Shader simple("shaders/simple.vs", "shaders/simple.fs");
	Shader gauroud("shaders/Gauroud.vs", "shaders/Gauroud.fs");
	Shader phong("shaders/Phong.vs", "shaders/Phong.fs");
	Shader blinn("shaders/Blinn.vs", "shaders/Blinn.fs");

	int shaderOption = Phong;

	fs::path file("models/nanosuit/nanosuit.obj");
	std::string absolutePath = fs::absolute(file).string();

	Model model(absolutePath);

	std::cout << "Model loading complete" << std::endl;

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, 900, 600);

	// Create a nanogui screen and pass the glfw pointer to initialize
	screen = new Screen();
	screen->initialize(window, true);


	glfwSetCursorPosCallback(window,
		[](GLFWwindow *, double x, double y) {
			screen->cursorPosCallbackEvent(x, y);
		}
	);

	glfwSetMouseButtonCallback(window,
		[](GLFWwindow *, int button, int action, int modifiers) {
			screen->mouseButtonCallbackEvent(button, action, modifiers);
		}
	);

	glfwSetKeyCallback(window,
		[](GLFWwindow *, int key, int scancode, int action, int mods) {
			screen->keyCallbackEvent(key, scancode, action, mods);
		}
	);


	// Create nanogui gui
	bool enabled = true;
	FormHelper *gui = new FormHelper(screen);
	ref<Window> nanoguiWindow = gui->addWindow(Eigen::Vector2i(10, 10), "Lighting Setting");

	gui->setFixedSize(Eigen::Vector2i(100, 30));
	gui->addGroup("Model");

	
	gui->addVariable("Color", colval)
		->setFinalCallback([&](const Color &c) {
			::lightColor = glm::vec3(c.r(), c.g(), c.b());
			});

	gui->addGroup("Lighting Model");
	auto options = gui->addVariable("Current", enumval, enabled);
	options->setCallback([&](int option) {
		shaderOption = option;
		});
	options->setItems({ "Phong", "Blinn", "Gauroud" });

	
	screen->setVisible(true);
	screen->performLayout();

	
	camera.MoveTo(0.0f, 5.0f, 5.0f);

	while (!glfwWindowShouldClose(window)) {
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		switch (shaderOption) {
		case Phong:
			phong.use();
			configShader(phong);
			glViewport(200, 0, 900, 600);
			model.Draw(phong);
			break;

		case Blinn:
			blinn.use();
			configShader(blinn);
			glViewport(200, 0, 900, 600);
			model.Draw(blinn);
			break;

		case Gauroud:
			gauroud.use();
			configShader(gauroud);
			glViewport(200, 0, 900, 600);
			model.Draw(gauroud);
			break;

		default:
			simple.use();
			configShader(simple);
			glViewport(200, 0, 900, 600);
			model.Draw(simple);
			break;
		}


		glViewport(0, 0, 100, 600);
		screen->drawContents();
		screen->drawWidgets();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void configShader(Shader& shader) {
	shader.setMat4("model", glm::mat4(1.0f));
	glm::mat4 view = camera.GetViewMatrix();
	shader.setMat4("view", view);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (900.0f - 200.0f) / 600.0f, 0.1f, 100.0f);
	shader.setVec3("lightColor", lightColor);
	shader.setMat4("projection", projection);
	shader.setVec3("diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("ambient", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setVec3("specular", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setFloat("shininess", 32.0f);
	shader.setVec3("cameraPos", camera.GetWorldPosition());
	shader.setVec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
}


void framebuffer_resize_callback(GLFWwindow* window, int x, int y) {
	glViewport(0, 0, x, y);
	screen->resizeCallbackEvent(x, y);
}

void UpdateMousePosition(double xpos, double ypos) {
	xPos = xpos;
	yPos = ypos;
}


void mouse_left_move_callback(GLFWwindow* window, double xpos, double ypos) {
	double xoffset = xpos - xPos;
	double yoffset = yPos - ypos;
	UpdateMousePosition(xpos, ypos);
	camera.MoveXZPlane(xoffset, yoffset, deltaTime);
}


void mouse_right_move_callback(GLFWwindow* window, double xpos, double ypos) {
	double xoffset = xpos - xPos;
	double yoffset = yPos - ypos;
	UpdateMousePosition(xpos, ypos);
	camera.Rotate(xoffset, -yoffset, deltaTime);
}


void mouse_middle_pressed_move_callback(GLFWwindow* window, double xpos, double ypos) {
	double xoffset = xpos - xPos;
	double yoffset = yPos - ypos;
	UpdateMousePosition(xpos, ypos);
	camera.MoveXYPlane(xoffset, -yoffset, deltaTime);
}


void mouse_middle_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.MoveYZPlane(10*xoffset, -yoffset * 10, deltaTime);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	switch (action) {
	case GLFW_PRESS:
		// record mosue position
		glfwGetCursorPos(window, &xPos, &yPos);
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			glfwSetCursorPosCallback(window, mouse_left_move_callback);
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			glfwSetCursorPosCallback(window, mouse_right_move_callback);
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			glfwSetCursorPosCallback(window, mouse_middle_pressed_move_callback);
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;
		default:
			break;
		}
		break;
	case GLFW_RELEASE:
		glfwSetCursorPosCallback(window, 0);
		break;
	default:
		break;
	}
}

