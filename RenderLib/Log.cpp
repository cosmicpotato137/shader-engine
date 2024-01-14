#include "Log.h"

// Empty wrapper function
std::function<void(const std::string &)> Console::handleOutput = [](const std::string &str) {
  std::ostringstream oss;
  oss << str;
};

void Console::SetCallback(std::function<void(const std::string &)> callback) 
{
  handleOutput = callback;
}

void Console::Log(const char *format, ...) {
  va_list args;
  va_start(args, format);

  char buffer[512];
  vsnprintf(buffer, sizeof(buffer), format, args);

  va_end(args);

  std::ostringstream ss;
  ss << "LOG::" << buffer;
  handleOutput(ss.str());
}

void Console::Warning(const char *format, ...) {
  va_list args;
  va_start(args, format);

  char buffer[512];
  vsnprintf(buffer, sizeof(buffer), format, args);

  va_end(args);

  std::ostringstream ss;
  ss << "WARNING::" << buffer;
  handleOutput(ss.str());
}

void Console::Error(const char *format, ...)  {
  va_list args;
  va_start(args, format);

  char buffer[512];
  vsnprintf(buffer, sizeof(buffer), format, args);

  va_end(args);

  std::ostringstream ss;
  ss << "ERROR::" << buffer;
  handleOutput(ss.str());
}

void Console::Assert(bool condition, const char *format, ...)  {
  if (condition) return;

  // Print error message with args
  va_list args;
  va_start(args, format);

  char buffer[512];
  vsnprintf(buffer, sizeof(buffer), format, args);

  va_end(args);

  std::ostringstream oss;
  oss << "EXCEPTION::" << buffer;

  throw std::exception(oss.str().c_str());
}
