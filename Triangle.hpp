#pragma once
#include "Object.hpp"
#include "global.hpp"
class Triangle : public Object {
   public:
    Vector3f v0, v1, v2;
    Vector3f e1, e2;
    Vector3f normal;
    Material* m;
    Triangle(Vector3f v0, Vector3f v1, Vector3f v2, Material* m = nullptr)
        : v0(v0), v1(v1), v2(v2), m(m) {
        e1 = v1 - v0;
        e2 = v2 - v0;
        normal = crossProduct(e1, e2);
    }
    bool intersect(const Ray& ray) {
        return getintersection(ray).happened;
    }
    Intersection getintersection(const Ray&);
    Bounds3 getbound();
    void getSurfaceProperties(const Vector3f&, const Vector3f&, const uint32_t&, const Vector2f&, Vector3f&, Vector2f&);
    Vector3f evalDiffuseColor(const Vector2f&);
};

Intersection Triangle::getintersection(const Ray& ray) {
    Intersection inter;

    if (dotProduct(ray.direction, normal) > 0)
        return inter;
    double u, v, t_tmp = 0;
    Vector3f pvec = crossProduct(ray.direction, e2);
    double det = dotProduct(e1, pvec);
    if (fabs(det) < EPS)
        return inter;

    double det_inv = 1. / det;
    Vector3f tvec = ray.origin - v0;
    u = dotProduct(tvec, pvec) * det_inv;
    if (u < 0 || u > 1)
        return inter;
    Vector3f qvec = crossProduct(tvec, e1);
    v = dotProduct(ray.direction, qvec) * det_inv;
    if (v < 0 || u + v > 1)
        return inter;
    t_tmp = dotProduct(e2, qvec) * det_inv;

    if (t_tmp < 0)
        return inter;
    inter.happened = true;
    inter.coords = ray.origin + t_tmp * ray.direction;
    inter.normal = this->normal;
    inter.distance = t_tmp;
    inter.obj = this;
    inter.m = this->m;
    return inter;
}

Bounds3 Triangle::getbound() {
    Bounds3 ret(Vector3f::Min(v0, Vector3f::Min(v1, v2)),
                Vector3f::Max(v0, Vector3f::Max(v1, v2)));
    return ret;
}

void Triangle::getSurfaceProperties(const Vector3f& P, const Vector3f& I, const uint32_t& index, const Vector2f& uv, Vector3f& N, Vector2f& st) {
    N = normalize(normal);
    st = Vector2f(0, 0);
}

Vector3f Triangle::evalDiffuseColor(const Vector2f& st) {
    return m->getColor();
}