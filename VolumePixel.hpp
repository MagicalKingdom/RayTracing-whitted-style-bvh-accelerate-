#pragma once
#ifndef __VOLUMEPIXEL
#define __VOLUMEPIXEL
#include "Object.hpp"
#include "global.hpp"

class VolumPixel : public Object {
   public:
    Vector3f p, length;
    Material* m;
    VolumPixel(const Vector3f& p, const Vector3f& length = Vector3f(3, 3, 3), Material* m = nullptr)
        : p(p), length(length), m(m) {
    }
    ~VolumPixel() {}
    bool intersect(const Ray&);
    Intersection getintersection(const Ray&);
    Bounds3 getbound();
    void getSurfaceProperties(const Vector3f&, const Vector3f&, const uint32_t&, const Vector2f&, Vector3f&, Vector2f&);
    Vector3f evalDiffuseColor(const Vector2f&);
};

bool VolumPixel::intersect(const Ray& ray) {
    Vector3f l, r;
    Vector3f pmin = p, pmax = p + length;
    for (int i = 0; i < 3; i++) {
        l[i] = (pmin[i] - ray.origin[i]) * (1.0 / ray.direction[i]);
        r[i] = (pmax[i] - ray.origin[i]) * (1.0 / ray.direction[i]);
        if (l[i] > r[i])
            std::swap(l[i], r[i]);
    }
    float l_max = l[0], r_min = r[0];
    l_max = std::max(l_max, std::max(l[1], l[2]));
    r_min = std::min(r_min, std::min(r[1], r[2]));
    if (l_max <= r_min && r_min > 0)
        return true;
    else
        return false;
}

Intersection VolumPixel::getintersection(const Ray& ray) {
    Vector3f l, r;
    Vector3f pmin = p, pmax = p + length;
    for (int i = 0; i < 3; i++) {
        l[i] = (pmin[i] - ray.origin[i]) * (1.0 / ray.direction[i]);
        r[i] = (pmax[i] - ray.origin[i]) * (1.0 / ray.direction[i]);
        if (l[i] > r[i])
            std::swap(l[i], r[i]);
    }
    float l_max = l[0], r_min = r[0];
    int l_max_idx = 1, r_min_idx = 1;
    for (int i = 1; i <= 2; i++) {
        if (l_max < l[i]) {
            l_max_idx = i;
            l_max = l[i];
        }
        if (r_min > r[i]) {
            r_min_idx = i;
            r_min = r[i];
        }
    }

    Intersection ret;
    if (!(l_max <= r_min && r_min > 0)) {
        return ret;
    }
    ret.happened = true;
    ret.obj = this;
    ret.m = this->m;
    if (l_max < 0) {
        ret.distance = r_min;
        ret.coords = ray.origin + r_min * ray.direction;
        if (r_min_idx == 1) {
            ret.normal = Vector3f(1, 0, 0);
        } else if (r_min_idx == 2) {
            ret.normal = Vector3f(0, 1, 0);
        } else {
            ret.normal = Vector3f(0, 0, 1);
        }
    } else {
        ret.distance = l_max;
        ret.coords = ray.origin + l_max * ray.direction;
        if (l_max_idx == 1) {
            ret.normal = Vector3f(-1, 0, 0);
        } else if (l_max_idx == 2) {
            ret.normal = Vector3f(0, -1, 0);
        } else {
            ret.normal = Vector3f(0, 0, -1);
        }
    }
    return ret;
}

Bounds3 VolumPixel::getbound() {
    return Bounds3(p, p + length);
}

void VolumPixel::getSurfaceProperties(const Vector3f& hitPoint, const Vector3f& ray_dir, const uint32_t& index, const Vector2f& uv, Vector3f& N, Vector2f& st) {
    st = Vector2f(0, 0);
    N = Vector3f(-1, 0, 0);
    auto pmin = p, pmax = p + length;
    float dist = fabs(hitPoint.x - pmin.x);
    if (dist > fabs(hitPoint.x - pmax.x)) {
        dist = fabs(hitPoint.x - pmax.x);
        N = Vector3f(1, 0, 0);
    }
    if (dist > fabs(hitPoint.y - pmin.y)) {
        dist = fabs(hitPoint.y - pmin.y);
        N = Vector3f(0, -1, 0);
    }
    if (dist > fabs(hitPoint.y - pmax.y)) {
        dist = fabs(hitPoint.y - pmax.y);
        N = Vector3f(0, 1, 0);
    }
    if (dist > fabs(hitPoint.z - pmin.z)) {
        dist = fabs(hitPoint.z - pmin.z);
        N = Vector3f(0, 0, -1);
    }
    if (dist > fabs(hitPoint.z - pmax.z)) {
        dist = fabs(hitPoint.z - pmax.z);
        N = Vector3f(0, 0, 1);
    }
}
Vector3f VolumPixel::evalDiffuseColor(const Vector2f& st) {
    return m->getColor();
}
#endif