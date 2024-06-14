#pragma once
#include "Object.hpp"
#include "global.hpp"
class Sphere : public Object {
   public:
    Vector3f center;
    float radius;
    Material* m;
    Sphere(const Vector3f&, const float&, Material* m);
    bool intersect(const Ray&);
    Intersection getintersection(const Ray&);
    Bounds3 getbound();
    void getSurfaceProperties(const Vector3f&, const Vector3f&, const uint32_t&, const Vector2f&, Vector3f&, Vector2f&);
    Vector3f evalDiffuseColor(const Vector2f&);
};
Sphere::Sphere(const Vector3f& center, const float& radius = 1, Material* m = nullptr)
    : center(center), radius(radius), m(m) {
}

bool Sphere::intersect(const Ray& ray) {
    Vector3f L = ray.origin - center;
    float a = dotProduct(ray.direction, ray.direction);
    float b = 2 * dotProduct(ray.direction, L);
    float c = dotProduct(L, L) - radius * radius;
    float t0, t1, dt = b * b - 4 * a * c;
    if (dt < 0) {
        return false;
    } else {
        t0 = (-b - sqrt(dt)) / (2 * a);
        t1 = (-b + sqrt(dt)) / (2 * a);
        if (t1 >= 0 || t0 >= 0)
            return true;
        else
            return false;
    }
}

Intersection Sphere::getintersection(const Ray& ray) {
    Vector3f L = ray.origin - center;
    float a = dotProduct(ray.direction, ray.direction);
    float b = 2 * dotProduct(ray.direction, L);
    float c = dotProduct(L, L) - radius * radius;
    float t0, t1, dt = b * b - 4 * a * c;
    Intersection ret;
    if (dt < 0) {
        return ret;
    } else {
        t0 = (-b - sqrt(dt)) / (2 * a);
        t1 = (-b + sqrt(dt)) / (2 * a);
        if (t0 < 0 && t1 < 0) {
            return ret;
        }
        if (t0 > t1) {
            std::swap(t0, t1);
        }
        if (t0 < 0) {
            t0 = t1;
        }
        ret.happened = true;
        ret.coords = ray.origin + t0 * ray.direction;
        ret.distance = ray.t;
        ret.m = this->m;
        ret.normal = normalize(ret.coords - this->radius);
        ret.obj = this;
        return ret;
    }
}

Bounds3 Sphere::getbound() {
    return Bounds3(Vector3f(center.x - radius - EPS, center.y - radius - EPS, center.z - radius - EPS),
                   Vector3f(center.x + radius + EPS, center.y + radius + EPS, center.z + radius + EPS));
}

void Sphere::getSurfaceProperties(const Vector3f& P, const Vector3f& I, const uint32_t& index, const Vector2f& uv, Vector3f& N, Vector2f& st) {
    N = normalize(P - center);
    st = Vector2f(0, 0);
}

Vector3f Sphere::evalDiffuseColor(const Vector2f& st) {
    return m->getColor();
}