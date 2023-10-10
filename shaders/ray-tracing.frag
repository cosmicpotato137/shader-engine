#version 450 core

in vec2 TexCoord;
out vec4 FragColor;

const int maxDepth = 3; // Maximum ray tracing depth

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct HitInfo {
    float t;
    vec3 point;
    vec3 normal;
};

// Box definition
struct Box {
    vec3 min;
    vec3 max;
};

bool intersectBox(Ray ray, Box box, inout HitInfo hit) {
    vec3 invDir = 1.0 / ray.direction;
    vec3 t1 = (box.min - ray.origin) * invDir;
    vec3 t2 = (box.max - ray.origin) * invDir;
    vec3 tmin = min(t1, t2);
    vec3 tmax = max(t1, t2);
    float tNear = max(max(tmin.x, tmin.y), tmin.z);
    float tFar = min(min(tmax.x, tmax.y), tmax.z);

    if (tNear > tFar || tFar < 0.0)
        return false;

    if (tNear > 0.0 && tNear < hit.t) {
        hit.t = tNear;
        hit.point = ray.origin + ray.direction * tNear;
        hit.normal = normalize(hit.point - (box.min + box.max) * 0.5); // Normal pointing outward
        return true;
    }

    return false;
}

vec3 trace(Ray ray, int depth) {
    if (depth >= maxDepth)
        return vec3(0.0);

    HitInfo hitBox;

    // Box
    Box box = Box(vec3(-0.5, -0.5, -0.5), vec3(0.5, 0.5, 0.5));
    bool hitBoxResult = intersectBox(ray, box, hitBox);

    if (hitBoxResult) {
        // Lambertian shading
        vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0)); // Directional light
        float diff = max(dot(normalize(hitBox.normal), lightDir), 0.0);
        vec3 objectColor = vec3(1.0, 0.0, 0.0); // Red box color
        vec3 lightColor = vec3(1.0, 1.0, 1.0); // White light color
        vec3 diffuse = diff * lightColor * objectColor;

        return diffuse;
    }

    // Background color
    return vec3(0.0, 0.0, 0.0); // Black background
}

void main() {
    // Screen coordinates to ray direction
    vec3 rayDir = normalize(vec3(TexCoord - 0.5, 1.0));
    Ray ray;
    ray.origin = vec3(0.0, 0.0, -3.0); // Camera position
    ray.direction = rayDir;

    // Ray tracing
    vec3 color = trace(ray, 0);

    FragColor = vec4(color, 1.0);
}
