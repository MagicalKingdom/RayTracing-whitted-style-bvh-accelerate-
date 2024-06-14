#pragma once
#include "Vector.hpp"
class Ray {
   public:
    Vector3f origin, direction;
    float t, tmin, tmax;
    Ray(const Vector3f& origin, const Vector3f& direction, const float& t = 0)
        : origin(origin), direction(direction) {
        tmin = 0;
        tmax = std::numeric_limits<float>::max();
    }
};