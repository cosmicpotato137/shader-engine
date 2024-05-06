#pragma once

// Graphics
// Note: must include glad before glfw3.h
#include <glad/gl.h>
#include <GLFW/glfw3.h>

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
#include <filesystem>
#include <fstream>

template <typename T> using ptr = std::shared_ptr<T>;
template <typename T> using uptr = std::unique_ptr<T>;