#pragma once
#include <array>
#include <limits>
#include "Intersection.hpp"
#include "Ray.hpp"
#include "Vector.hpp"
class Bounds3 {
   public:
    Vector3f pmin, pmax;
    Bounds3(Vector3f pmin, Vector3f pmax)
        : pmin(pmin), pmax(pmax) {}
    Bounds3() {
        pmin.x = pmin.y = pmin.z = std::numeric_limits<float>::max();
        pmax.x = pmax.y = pmax.z = std::numeric_limits<float>::lowest();
    }
    static Bounds3 Union(const Bounds3& a, const Bounds3& b) {
        return Bounds3(Vector3f::Min(a.pmin, b.pmin), Vector3f::Max(a.pmax, b.pmax));
    }
    Bounds3 Union(const Bounds3& a) {
        return Bounds3(Vector3f::Min(a.pmin, pmin), Vector3f::Max(a.pmax, pmax));
    }

    bool isIntersection(const Ray& ray) {
        Vector3f l, r;
        for (int i = 0; i < 3; i++) {
            l[i] = (this->pmin[i] - ray.origin[i]) * (1.0 / ray.direction[i]);
            r[i] = (this->pmax[i] - ray.origin[i]) * (1.0 / ray.direction[i]);
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
};
