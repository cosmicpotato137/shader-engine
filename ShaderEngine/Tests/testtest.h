
// Serialize base class pointer
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include <fstream>
#include <iostream>
#include <string>

std::string path =
    "C:/Users/imacs/Documents/Code/shader-engine/bin/Debug/data/";

class AbstractPoint {
public:
  virtual ~AbstractPoint() {}
  virtual void DoSomething() = 0;

private:
  friend class boost::serialization::access;
  // This is required if we want to serialize an AbstractPoint pointer
  template <class Archive>
  void serialize(Archive &, const unsigned int /*version*/) {
    // do nothing
  }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(AbstractPoint)
BOOST_CLASS_EXPORT(AbstractPoint)

class Point : public AbstractPoint {
public:
  Point() = default;
  Point(const double data) : mData(data) {}

  void DoSomething() {}

  double mData;

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &archive, const unsigned int /*version*/) {
    archive &boost::serialization::base_object<AbstractPoint>(*this) & mData;
  }
};

BOOST_CLASS_EXPORT(Point)