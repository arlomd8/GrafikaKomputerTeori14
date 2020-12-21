// NAMA : ARLO MARIO DENDI
// NRP	: 4210181018


#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <glew.h> //GLEW
#include <glfw3.h> // GLFW

//GLM
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include <SOIL2.h> // SOIL2

using namespace std;
using namespace glm;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
};