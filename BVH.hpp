#pragma once
#ifndef __BVH
#define __BVH
#include <queue>
#include "Bound.hpp"
#include "Intersection.hpp"
#include "Object.hpp"
class BVH_node {
   public:
    BVH_node *left, *right;
    Bounds3 box;
    Object* obj;
    BVH_node();

    BVH_node(const Bounds3& box);

    bool ray_intersect_box(const Ray& ray);
    Intersection ray_intersect_obj(const Ray& ray);
};

class BVH {
   public:
    BVH_node* root;
    BVH();
    ~BVH();
    void build(std::vector<Object*> obj);
    Intersection intersect(const Ray& ray);

   private:
    BVH_node* build(std::vector<Object*> obj, int step);
    Intersection intersect(BVH_node* r, const Ray& ray);
};

/*************************************************************/
// 比较函数
inline bool cmp_obj_x(Object*& a, Object*& b);
inline bool cmp_obj_y(Object*& a, Object*& b);
inline bool cmp_obj_z(Object*& a, Object*& b);
/***************************************************************/

#endif