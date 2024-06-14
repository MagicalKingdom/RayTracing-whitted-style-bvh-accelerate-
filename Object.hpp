#pragma once
#ifndef __OBJECT
#define __OBJECT

#include "Bound.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"
#include "Vector.hpp"
#include "global.hpp"
class Object {
   public:
    Object() {}
    virtual ~Object() {}
    virtual bool intersect(const Ray&) = 0;
    virtual Intersection getintersection(const Ray&) = 0;
    virtual Bounds3 getbound() = 0;
    virtual void getSurfaceProperties(const Vector3f&, const Vector3f&, const uint32_t&, const Vector2f&, Vector3f&, Vector2f&) = 0;
    virtual Vector3f evalDiffuseColor(const Vector2f&) = 0;
};

#endif