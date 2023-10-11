#include "core.h"

#include "Camera.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

class RenderObject {
public:
	ptr<Transform> transform;
	ptr<Mesh> mesh;
	ptr<Material> material;

	RenderObject(ptr<Transform> transf, ptr<Mesh> mesh, ptr<Material> mat) : transform(transf), mesh(mesh), material(mat) {
		
	}
};