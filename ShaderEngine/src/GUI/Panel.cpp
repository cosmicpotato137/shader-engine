#include "pch.h"
#include "Panel.h"

Panel &Panel::operator=(const Panel &other) {
  m_Name = other.m_Name;
  m_Size = other.m_Size;
  return *this;
}