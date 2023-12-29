#pragma once
#include "core.h"

template <typename T> class ComputeBuffer1D {
  GLuint buffer;
  int size;
  std::vector<T> data;

public:
  ComputeBuffer1D();
  ~ComputeBuffer1D();

  void Init();
  void InitFromVector(const std::vector<T> data);

  void PushBack(T element);
  T GetElement(int index);

  void BindCompute(int location);

  GLuint GetBufferID() const;
  int GetSize() const;

  void Cleanup();

private:
  void InitFromData();
};

template <typename T>
ComputeBuffer1D<T>::ComputeBuffer1D() : size(0), buffer(0) {
  data = std::vector<T>{};
}

template <typename T> ComputeBuffer1D<T>::~ComputeBuffer1D() { Cleanup(); }

template <typename T> void ComputeBuffer1D<T>::Init() {

  this->size = data.size();

  // Create and bind a buffer object
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);

  // Allocate storage for the buffer
  glBufferData(
      GL_SHADER_STORAGE_BUFFER, size * sizeof(T), nullptr, GL_DYNAMIC_DRAW);

  // Bind the buffer to a binding point (you can customize the binding point)
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffer);
}

template <typename T>
void ComputeBuffer1D<T>::InitFromVector(const std::vector<T> data) {
  this->data = data;
  InitFromData();
}

// Copy a single element into the buffer
template <typename T> void ComputeBuffer1D<T>::PushBack(T element) {
  data.push_back(element);
  if (data.size() > size)
    InitFromData();
  else {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glBufferSubData(
        GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(T), sizeof(T), &element);
  }
}

template <typename T> T ComputeBuffer1D<T>::GetElement(int index) {
  return data[index];
}

template <typename T> void ComputeBuffer1D<T>::BindCompute(int location) {
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, buffer);
}

template <typename T> GLuint ComputeBuffer1D<T>::GetBufferID() const {
  return buffer;
}

template <typename T> int ComputeBuffer1D<T>::GetSize() const { return size; }

template <typename T> void ComputeBuffer1D<T>::Cleanup() {
  // Delete the buffer object
  glDeleteBuffers(1, &buffer);
}

template <typename T> void ComputeBuffer1D<T>::InitFromData() {
  Init();

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
  glBufferSubData(
      GL_SHADER_STORAGE_BUFFER, 0, data.size() * sizeof(T), data.data());
}