#include "Libs.h"

Vertex vertices[] =
{
	//Position						//Color						//Texcoords
	vec3(-0.5f, 0.5f,0.0f),			vec3(1.0f, 0.0f, 0.0f),		vec2(0.0f,1.0f), // 0
	vec3(-0.5f, -0.5f, 0.0f),		vec3(0.0f, 1.0f ,0.0f),		vec2(0.0f,0.0f), // 1
	vec3(0.5f, -0.5f, 0.0f),		vec3(0.0f, 0.0f, 1.0f),		vec2(1.0f,0.0f), // 2

	//vec3(-0.5f, 0.5f,0.0f),			vec3(1.0f, 0.0f, 0.0f),		vec2(0.0f,1.0f),
	//vec3(0.5f, -0.5f, 0.0f),		vec3(0.0f, 0.0f, 1.0f),		vec2(1.0f,0.0f),
	vec3(0.5f, 0.5f, 0.0f),			vec3(0.0f, 1.0f ,1.0f),		vec2(1.0f,1.0f)	 //3
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = 
{
	0, 1, 2, //Triangle 1
	0, 2, 3	//Triangle 2
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
	const int WINDOW_WIDTH = 720;
	const int WINDOW_HEIGHT = 720;
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


	// INIT TEXTURE
	int image_width = 0;
	int image_height = 0;
	unsigned char* image = SOIL_load_image("Textures/Logo.png", &image_width, &image_height,NULL,SOIL_LOAD_RGBA);

	GLuint texture0; //texture ID
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";	
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0); 
	SOIL_free_image_data(image);

	// TEXTURE 1
	int image_width1 = 0;
	int image_height1 = 0;
	unsigned char* image1 = SOIL_load_image("Textures/Space.png", &image_width1, &image_height1, NULL, SOIL_LOAD_RGBA);

	GLuint texture1; //texture ID
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width1, image_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image1);

	//MAIN LOOP
	while (!glfwWindowShouldClose(window)) {
		//INPUT UPDATE
		glfwPollEvents();

		//UPDATE --
		UpdateInput(window);

		//DRAW --


		//CLEAR
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//USE A PROGRAM
		glUseProgram(core_program);

		//update uniforms
		glUniform1i(glGetUniformLocation(core_program, "texture0"), 0);
		glUniform1i(glGetUniformLocation(core_program, "texture1"), 1);
	
		//ACTIVATE TEXTURE
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		//Bind Vertex Array Object
		glBindVertexArray(VAO);

		//Draw
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);	

		//END DRAW ( RENDER )
		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// END PROGRAM
	glfwDestroyWindow(window);
	glfwTerminate();
	glDeleteProgram(core_program);


	//system("pause");
	return 0;
}