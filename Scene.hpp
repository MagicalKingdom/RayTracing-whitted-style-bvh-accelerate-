#pragma once
#include <vector>
#include "BVH.hpp"
#include "Intersection.hpp"
#include "Light.hpp"
#include "Object.hpp"
#include "Ray.hpp"
#include "Vector.hpp"
class Scene {
   public:
    // 屏幕大小
    int width = 1280, height = 960;
    // 俯仰角
    float fov = 90;
    // 摄像头位置
    Vector3f eye_pos = Vector3f(0.0f, 0.0f, 0.0f);
    // 摄像头对着的方向
    Vector3f eye_direction = Vector3f(0.0f, 0.0f, -1.0f);
    // 背景颜色
    Vector3f backgroundColor = Vector3f(0.0f, 0.0f, 0.0f);
    // 光线最大递归深度
    int maxDepth = 8;
    // 物体指针
    std::vector<Object*> obj;
    // 光照指针
    std::vector<Light*> light;
    // BVH树
    BVH bvh;

    float gamma = 1.0f;

    // Scene(int width=1280,int height=960,float fov=90,)
    void add_obj(Object* p) {
        obj.push_back(p);
    }
    void add_light(Light* _light) {
        light.push_back(_light);
    }
    void build_bvh() {
        bvh.build(obj);
    }

    Intersection intersection(const Ray& ray) {
        return this->bvh.intersect(ray);
    }
    Vector3f castRay(const Ray& ray, const int& depth);
};