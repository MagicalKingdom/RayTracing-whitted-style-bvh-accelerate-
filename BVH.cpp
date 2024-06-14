#include "BVH.hpp"
#include <queue>
#include "Bound.hpp"
#include "Intersection.hpp"
#include "Object.hpp"

BVH_node::BVH_node() {
    left = right = nullptr;
    obj = nullptr;
}
BVH_node::BVH_node(const Bounds3& box)
    : box(box) {
    left = right = nullptr;
    obj = nullptr;
}

bool BVH_node::ray_intersect_box(const Ray& ray) {
    return box.isIntersection(ray);
}

Intersection BVH_node::ray_intersect_obj(const Ray& ray) {
    Intersection ret;
    if (obj == nullptr) {
        return ret;
    } else {
        return obj->getintersection(ray);
    }
}

BVH::BVH() {
    root = nullptr;
}

BVH::~BVH() {
    std::queue<BVH_node*> q;
    if (root == nullptr)
        return;
    q.push(root);
    while (!q.empty()) {
        auto now = q.front();
        q.pop();
        if (now->left != nullptr) {
            q.push(now->left);
        }
        if (now->right != nullptr) {
            q.push(now->right);
        }
        delete now;
    }
    root = nullptr;
}

void BVH::build(std::vector<Object*> obj) {
    root = build(obj, 0);
}

BVH_node* BVH::build(std::vector<Object*> obj, int step) {
    static int debug_tag = 0;
    auto size = obj.size();
    if (size == 0) {
        return nullptr;
    } else if (size == 1) {
        auto ret = new BVH_node(obj[0]->getbound());
        ret->obj = obj[0];
        debug_tag++;
        std::cerr << debug_tag << std::endl;
        return ret;
    } else {
        step = (step + 1) % 3;
        if (step == 0) {
            // 分割x
            sort(obj.begin(), obj.end(), cmp_obj_x);
            auto sz = obj.size();
            sz /= 2;
            std::vector<Object*> left_obj(obj.begin(), obj.begin() + sz);
            std::vector<Object*> right_obj(obj.begin() + sz, obj.end());
            BVH_node* left = build(left_obj, step);
            BVH_node* right = build(right_obj, step);
            BVH_node* ret = new BVH_node();
            ret->left = left;
            ret->right = right;
            ret->box = left->box.Union(right->box);
            return ret;
        } else if (step == 1) {
            // 分割y
            sort(obj.begin(), obj.end(), cmp_obj_y);
            auto sz = obj.size();
            sz /= 2;
            std::vector<Object*> left_obj(obj.begin(), obj.begin() + sz);
            std::vector<Object*> right_obj(obj.begin() + sz, obj.end());
            BVH_node* left = build(left_obj, step);
            BVH_node* right = build(right_obj, step);
            BVH_node* ret = new BVH_node();
            ret->left = left;
            ret->right = right;
            ret->box = left->box.Union(right->box);
            return ret;
        } else {
            // 分割z
            sort(obj.begin(), obj.end(), cmp_obj_z);
            auto sz = obj.size();
            sz /= 2;
            std::vector<Object*> left_obj(obj.begin(), obj.begin() + sz);
            std::vector<Object*> right_obj(obj.begin() + sz, obj.end());
            BVH_node* left = build(left_obj, step);
            BVH_node* right = build(right_obj, step);
            BVH_node* ret = new BVH_node();
            ret->left = left;
            ret->right = right;
            ret->box = left->box.Union(right->box);
            return ret;
        }
    }
}

Intersection BVH::intersect(BVH_node* r, const Ray& ray) {
    Intersection ret;
    if (r == nullptr || !(r->box.isIntersection(ray))) {
        return ret;
    }
    if (r->left == nullptr && r->right == nullptr) {
        return r->obj->getintersection(ray);
    }
    auto le = intersect(r->left, ray);
    auto ri = intersect(r->right, ray);
    if (le.happened && ri.happened) {
        if (le.distance < ri.distance)
            return le;
        else
            return ri;
    } else if (le.happened) {
        return le;
    } else if (ri.happened) {
        return ri;
    } else {
        return ret;
    }
}

Intersection BVH::intersect(const Ray& ray) {
    Intersection ret;
    if (root == nullptr) {
        return ret;
    } else {
        return this->intersect(root, ray);
    }
}

inline bool cmp_obj_x(Object*& a, Object*& b) {
    auto pp = a->getbound(), qq = b->getbound();
    return pp.pmin.x + pp.pmax.x <= qq.pmin.x + qq.pmax.x;
}
inline bool cmp_obj_y(Object*& a, Object*& b) {
    auto pp = a->getbound(), qq = b->getbound();
    return pp.pmin.y + pp.pmax.y <= qq.pmin.y + qq.pmax.y;
}
inline bool cmp_obj_z(Object*& a, Object*& b) {
    auto pp = a->getbound(), qq = b->getbound();
    return pp.pmin.z + pp.pmax.z <= qq.pmin.z + qq.pmax.z;
}