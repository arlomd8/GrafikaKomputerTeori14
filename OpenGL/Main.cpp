#include "Libs.h"

Vertex vertices[] =
{
	//Position						//Color						//Texcoords
	vec3(0.0f, 0.5f,0.0f),			vec3(1.0f, 0.0f, 0.0f),		vec2(0.0f,1.0f),
	vec3(-0.5f, -0.5f, 0.0f),		vec3(0.0f, 1.0f ,0.0f),		vec2(0.0f,0.0f),
	vec3(0.5f, -0.5f, 0.0f),		vec3(0.0f, 0.0f, 1.0f),		vec2(1.0f,0.0f) 
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = 
{
	0, 1, 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

void UpdateInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void FrameBuffer_Resize_Callback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

bool LoadShaders(GLuint& program)
{
	bool loadSuccess = true;
	char infoLog[512];
	GLint success;

	string temp = "";
	string src = "";

	ifstream in_file;

	// load vertex
	in_file.open("VertexShader.glsl");

	if (in_file.is_open()) 
	{
		while (getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{
		cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE" << "\n";
		loadSuccess = false;
	}
		

	in_file.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertSrc = src.c_str();
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_VERTEX_SHADER" << "\n";
		cout << infoLog << "\n";
		loadSuccess = false; 
	}

	temp = "";
	src = "";

	// LOAD FRAGMENT
	in_file.open("FragmentShader.glsl");

	if (in_file.is_open())
	{
		while (getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{
		cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_FRAGMENT_FILE" << "\n";
		loadSuccess = false;
	}
		
	in_file.close();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragSrc = src.c_str();
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader); 

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_FRAGMENT_SHADER" << "\n";
		cout << infoLog << "\n";
		loadSuccess = false;
	}

	temp = "";
	src = "";

	// program
	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cout << "ERROR::LOADSHADERS::COULD_NOT_LINK_PROGRAM" << "\n";
		cout << infoLog << "\n";
		loadSuccess = false;
	}

	// end
	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return loadSuccess;
}

int main() {

	// INIT GLFW
	glfwInit();

	// create window
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;
	 
	// create context version 4.4
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

	// OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE); //memisahkan backface
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND); // blend color
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



	// INIT SHADER
	GLuint core_program;
	if(!LoadShaders(core_program))
	{
		glfwTerminate(); 
	}


	// VAO, VBO, EBO

	//generate vao dan bind
	GLuint VAO; 
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO); 

	//generate vbo and bind and send data
	GLuint VBO; 
	glGenBuffers(1, &VBO); //generate
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //send vertex data

	//generate ebo and bind and send data
	GLuint EBO;
	glGenBuffers(1, &EBO); //generate
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //send index/element data
	

	// SET VERTEX ATTRIBPOINTERS AND ENABLE (input assembly)

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	 
	//texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	//BIND VAO 0
	glBindVertexArray(0); 


	//MAIN LOOP
	while (!glfwWindowShouldClose(window)) {
		//INPUT UPDATE
		glfwPollEvents();

		//UPDATE --
		UpdateInput(window);

		//DRAW --


		//CLEAR
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//USE A PROGRAM
		glUseProgram(core_program);

		//Bind Vertex Array Object
		glBindVertexArray(VAO);

		//Draw
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);	

		//END DRAW ( RENDER )
		glfwSwapBuffers(window);
		glFlush();
	}

	// END PROGRAM
	glfwDestroyWindow(window);
	glfwTerminate();
	glDeleteProgram(core_program);


	//system("pause");
	return 0;
}