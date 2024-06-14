#pragma once
#ifndef __INTERSECTION
#define __INTERSECTION
#include "Material.hpp"
#include "Vector.hpp"

class Object;
class Intersection {
   public:
    Intersection() {
        happened = false;
        coords = Vector3f();
        normal = Vector3f();
        distance = std::numeric_limits<double>::max();
        obj = nullptr;
        m = nullptr;
    }
    bool happened;
    Vector3f coords;
    Vector3f normal;
    double distance;
    Object* obj;
    Material* m;
};

#endif