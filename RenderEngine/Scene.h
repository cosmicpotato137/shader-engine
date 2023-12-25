#include "re_core.h"

struct Scene {
  std::string name;

  Scene(const std::string &name) : name(name) {}

  virtual void Start() = 0;
  virtual void Update(double dt) = 0;
}