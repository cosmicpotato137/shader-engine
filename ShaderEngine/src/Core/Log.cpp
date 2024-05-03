#include "pch.h"
#include "Log.h"

// Empty wrapper function
std::function<void(const std::string &)> Console::handleOutput =
    [](const std::string &str) { std::cout << str << "\n"; };

void Console::SetCallback(std::function<void(const std::string &)> callback) {
  handleOutput = callback;
}

// arg formatting for logging functions
#define FORMAT_ARGS(x)                               \
  va_list args;                                      \
  va_start(args, format);                            \
  char _buffer[512];                                 \
  vsnprintf(_buffer, sizeof(_buffer), format, args); \
  va_end(args);                                      \
  x = std::string(_buffer)

void Console::Log(const char *format, ...) {
  std::string buffer;
  FORMAT_ARGS(buffer);
  handleOutput("[LOG]       " + buffer);
}

void Console::Warning(const char *format, ...) {
  std::string buffer;
  FORMAT_ARGS(buffer);
  handleOutput("[WARNING]   " + buffer);
}

void Console::Error(const char *format, ...) {
  std::string buffer;
  FORMAT_ARGS(buffer);
  handleOutput("[ERROR]     " + buffer);
}

void Console::Assert(bool condition, const char *format, ...) {
  if (condition)
    return;

  std::string buffer;
  FORMAT_ARGS(buffer);

  std::ostringstream oss;
  oss << "[EXCEPTION] " << buffer;

  throw std::exception(oss.str().c_str());
}

#undef FORMAT_ARGS