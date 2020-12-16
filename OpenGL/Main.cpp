#include "Libs.h"

void FrameBuffer_Resize_Callback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

int main() {

	// INIT GLFW
	glfwInit();

	// CREATE WINDOW
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;
	 
	//CREATE CONTEXT version 4.4
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "4210181018", NULL, NULL);

	glfwSetFramebufferSizeCallback(window, FrameBuffer_Resize_Callback);
	//glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(window);

	// INIT GLEW (NEED WINDOWS AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	//MAIN LOOP
	while (!glfwWindowShouldClose(window)) {
		//INPUT UPDATE
		glfwPollEvents();

		//UPDATE
		//DRAW


		//CLEAR
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//END DRAW ( RENDER )
		glfwSwapBuffers(window);
		glFlush();
	}

	// END PROGRAM
	glfwTerminate();

	system("pause");
	return 0;
}