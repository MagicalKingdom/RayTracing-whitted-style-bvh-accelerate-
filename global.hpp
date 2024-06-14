#pragma once
#ifndef __GLOBAL
#define __GLOBAL
#include <cmath>
#include <iostream>
#include <random>
#include "Vector.hpp"
#undef M_PI
#define M_PI 3.141592653589793f
extern const float EPS;
inline float get_random_float();

// 反射方向
Vector3f reflect(const Vector3f& I, const Vector3f& N);

// 折射方向
Vector3f refract(const Vector3f& I, const Vector3f& N, const float& ior);

// 反射率
float fresnel(const Vector3f& I, const Vector3f& N, const float& ior);

float deg2rad(const float& deg);

#endif