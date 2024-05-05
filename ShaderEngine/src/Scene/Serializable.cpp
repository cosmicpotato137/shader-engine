#include "pch.h"
#include "Serializable.h"
#include "Core/Log.h"

using namespace boost;

void Serializable::Save(const std::string &filepath) {
  std::ofstream ofs(filepath);
  archive::text_oarchive oa(ofs);
  oa << *this;
}

Serializable *Serializable::Load(const std::string &filepath) {
  Serializable *obj;
  std::ifstream ifs(filepath);
  archive::text_iarchive ia(ifs);
  ia >> obj;
  return obj;
}