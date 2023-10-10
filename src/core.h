#pragma once

// graphics
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// std lib
#include <vector>
#include <string>
#include <iostream>
#include <map>

template<typename T> 
using ptr = std::shared_ptr<T>;