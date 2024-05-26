#pragma once

#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>

#include <streambuf>

#include <functional>

class Application;

// Debugging macros that print to the console;
class Console : public std::streambuf {
  static std::function<void(const std::string &)> handleOutput;

public:
  static void SetCallback(std::function<void(const std::string &)> callback);

  static void Log(const char *format, ...);

  static void Warning(const char *format, ...);

  static void Error(const char *format, ...);

  static void Assert(bool condition, const char *format, ...);
};