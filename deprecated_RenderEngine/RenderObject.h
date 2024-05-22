#pragma once

#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"
#include "Object.h"

class RenderObject : public Object {
public:
  ptr<Transform> transform;
  ptr<Mesh> mesh;
  ptr<Material> material;

  RenderObject(ptr<Transform> transf, ptr<Mesh> mesh, ptr<Material> mat)
      : transform(transf), mesh(mesh), material(mat) {}
};