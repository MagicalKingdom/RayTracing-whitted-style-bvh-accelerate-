#pragma once

#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

#include "Vector.hpp"

enum MaterialType { DIFFUSE_AND_GLOSSY,
                    REFLECTION_AND_REFRACTION,
                    REFLECTION };

class Material {
   public:
    MaterialType m_type;
    Vector3f m_color;
    Vector3f m_emission;  // 自发光
    float ior;
    float Kd, Ks;
    float specularExponent;
    // Texture tex;

    inline Material(MaterialType t = DIFFUSE_AND_GLOSSY, Vector3f c = Vector3f(1, 1, 1), Vector3f e = Vector3f(0, 0, 0));
    inline MaterialType getType();
    inline Vector3f getColor();
    inline Vector3f getColorAt(double u, double v);
    inline Vector3f getEmission();
};

Material::Material(MaterialType t, Vector3f c, Vector3f e) {
    m_type = t;
    m_color = c;
    m_emission = e;
    Kd = 0.8;
    Ks = 0.2;
    ior = 1.5;
    specularExponent = 25;
}

MaterialType Material::getType() {
    return m_type;
}
Vector3f Material::getColor() {
    return m_color;
}
Vector3f Material::getEmission() {
    return m_emission;
}

Vector3f Material::getColorAt(double u, double v) {
    return Vector3f();
}
#endif
