#pragma once
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class Serializable {
public:
  void Save(const std::string &filepath);
  static Serializable *Load(const std::string &filepath);

protected:
  friend class boost::serialization::access;
  virtual void
  serialize(boost::archive::text_oarchive &ar, const unsigned int version) = 0;
  virtual void
  serialize(boost::archive::text_iarchive &ar, const unsigned int version) = 0;
  // virtual void Serialize(std::ofstream &ofs) = 0;
  // virtual void Deserialize(std::ifstream &ifs) = 0;
};