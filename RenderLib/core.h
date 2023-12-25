#pragma once

// Graphics
#include <GLFW/glfw3.h>
#include <glad/gl.h>

// Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

// Std lib
#include <iostream>
#include <map>
#include <string>
#include <vector>

template <typename T> using ptr = std::shared_ptr<T>;