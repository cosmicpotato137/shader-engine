#pragma once

#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <string>

// Debugging macros that print to the console;
class Console {
public:
  static void Log(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("LOG::");
    vprintf(format, args);
    printf("\n");
    va_end(args);
  }

  static void Warning(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "WARNING::");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
  }

  static void Error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "ERROR::");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
  }
};