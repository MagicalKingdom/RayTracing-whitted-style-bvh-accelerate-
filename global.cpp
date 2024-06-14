#include "global.hpp"
#include <cmath>
#include <iostream>
#include <random>
#include "Vector.hpp"
#undef M_PI
#define M_PI 3.141592653589793f
const float EPS = 0.00001;
inline float get_random_float() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    return dist(rng);
}

// 反射方向
Vector3f reflect(const Vector3f& I, const Vector3f& N) {
    return I - 2 * dotProduct(I, N) * N;
}

// 折射方向
Vector3f refract(const Vector3f& I, const Vector3f& N, const float& ior) {
    float cosi = dotProduct(I, N);
    cosi = std::max(-1.0f, std::min(1.0f, cosi));
    float etai = 1, etat = ior;
    Vector3f n = N;
    if (cosi < 0) {
        cosi = -cosi;
    } else {
        std::swap(etai, etat);
        n = -N;
    }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
}

// 反射率
float fresnel(const Vector3f& I, const Vector3f& N, const float& ior) {
    float cosi = std::clamp(dotProduct(I, N), -1.0f, 1.0f);
    float etai = 1, etat = ior;
    if (cosi > 0) {
        std::swap(etai, etat);
    }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        return 1;
    } else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        return (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
}

float deg2rad(const float& deg) {
    return deg * M_PI / 180.0;
}