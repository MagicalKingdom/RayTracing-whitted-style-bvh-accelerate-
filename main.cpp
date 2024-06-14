#include <iostream>
#include "Material.hpp"
#include "Rander.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "Vector.hpp"
#include "VolumePixel.hpp"
int main() {
    std::cerr << "start" << std::endl;
    Scene scene;
    Vector3f p0(-20, -15, 10), p1(20, 15, -50);
    // Vector3f p0(-4, -3, 0), p1(4, 3, -20);
    Vector3f v[8];
    v[0].x = v[1].x = v[4].x = v[5].x = p1.x;
    v[3].x = v[7].x = v[6].x = v[2].x = p0.x;
    v[4].y = v[5].y = v[6].y = v[7].y = p1.y;
    v[0].y = v[1].y = v[2].y = v[3].y = p0.y;
    v[1].z = v[2].z = v[5].z = v[6].z = p1.z;
    v[0].z = v[3].z = v[4].z = v[7].z = p0.z;

    Material m_blue(DIFFUSE_AND_GLOSSY, Vector3f(0, 0, 0.7));
    Material m_red(DIFFUSE_AND_GLOSSY, Vector3f(0.7, 0, 0));
    Material m_yellow(DIFFUSE_AND_GLOSSY, Vector3f(0.7, 0.7, 0));
    Material m_white(DIFFUSE_AND_GLOSSY, Vector3f(0.7, 0.7, 0.7));

    Material m_green(DIFFUSE_AND_GLOSSY, Vector3f(0, 0.2, 0));
    Material m_g(DIFFUSE_AND_GLOSSY, Vector3f(0, 0.2, 0.2));
    Triangle* pt[12];
    for (int i = 0; i < 12; i++) {
        pt[i] = nullptr;
    }
    // 右
    pt[0] = new Triangle(v[0], v[4], v[1], &m_blue);
    pt[1] = new Triangle(v[4], v[5], v[1], &m_blue);
    // 左
    pt[4] = new Triangle(v[2], v[6], v[7], &m_red);
    pt[5] = new Triangle(v[3], v[2], v[7], &m_red);
    // 上
    pt[6] = new Triangle(v[4], v[6], v[5], &m_yellow);
    pt[7] = new Triangle(v[6], v[4], v[7], &m_yellow);
    // 下
    pt[8] = new Triangle(v[0], v[1], v[2], &m_white);
    pt[9] = new Triangle(v[0], v[2], v[3], &m_white);

    // 远
    pt[2] = new Triangle(v[1], v[5], v[2], &m_green);
    pt[3] = new Triangle(v[5], v[6], v[2], &m_green);
    // 近
    pt[10] = new Triangle(v[7], v[4], v[0], &m_white);
    pt[11] = new Triangle(v[7], v[0], v[3], &m_yellow);

    for (int i = 0; i < 12; i++) {
        if (pt[i] != nullptr)
            scene.add_obj(pt[i]);
    }

    Material m1(REFLECTION);
    m1.m_color = Vector3f(0, 1, 0);
    Sphere sphere1(Vector3f(10, -5, -30), 10, &m1);
    scene.add_obj(&sphere1);

    Material m2(DIFFUSE_AND_GLOSSY);
    m2.m_color = Vector3f(0.5, 0.5, 0);
    Sphere sphere2(Vector3f(-11.5, -2, -22), 3, &m2);
    scene.add_obj(&sphere2);

    VolumPixel volumpixel(Vector3f(-15, -15, -25), Vector3f(7, 10, 6), &m_blue);
    scene.add_obj(&volumpixel);

    Light light1(Vector3f(0, 14, -25), 1.0);
    scene.add_light(&light1);

    Light light2(Vector3f(0, 0, 0), 0.13);
    scene.add_light(&light2);

    scene.backgroundColor = Vector3f(0.3, 0.3, 0.3);

    scene.build_bvh();
    scene.gamma = 0.8f;
    Rander::rander(scene);

    for (int i = 0; i < 12; i++) {
        delete pt[i];
    }

    return 0;
}