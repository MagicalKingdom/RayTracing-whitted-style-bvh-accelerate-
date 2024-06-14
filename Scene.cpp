#include "Scene.hpp"
#include "global.hpp"
Vector3f Scene::castRay(const Ray& ray, const int& depth) {
    if (depth > this->maxDepth) {
        return Vector3f(0.0, 0.0, 0.0);
    }
    Intersection intersection = this->intersection(ray);
    Material* m = intersection.m;
    Object* hitObject = intersection.obj;
    Vector3f hitColor = this->backgroundColor;
    Vector2f uv;
    uint32_t index = 0;
    if (intersection.happened) {
        Vector3f hitPoint = intersection.coords;
        Vector3f N = normalize(intersection.normal);
        Vector2f st = Vector2f(0, 0);  // st coordinates
        hitObject->getSurfaceProperties(hitPoint, ray.direction, index, uv, N, st);
        // std::cerr << "happened" << std::endl;
        if (m->getType() == REFLECTION_AND_REFRACTION) {
            // 反射也折射

            // 反射方向
            Vector3f reflectionDir = normalize(reflect(ray.direction, N));
            // 折射方向
            Vector3f refractionDir = normalize(refract(ray.direction, N, m->ior));
            // 反射起点，折射起点，添加一个小的偏移量防止自相交
            Vector3f reflectionOrig = dotProduct(N, reflectionDir) >= 0 ? hitPoint + N * EPS : hitPoint - N * EPS;
            Vector3f refractionOrig = dotProduct(N, refractionDir) >= 0 ? hitPoint + N * EPS : hitPoint - N * EPS;
            // 递归地计算反射和折射
            Vector3f reflectionColor = castRay(Ray(reflectionOrig, reflectionDir), depth + 1);
            Vector3f refractionColor = castRay(Ray(refractionOrig, refractionDir), depth + 1);
            // 计算反射率
            float k = fresnel(ray.direction, N, m->ior);
            k = 0.3;
            // k = 0.01;
            //  计算最终的颜色
            hitColor = k * reflectionColor + (1.0 - k) * refractionColor;
        } else if (m->getType() == REFLECTION) {
            // 仅反射

            // 反射方向
            Vector3f reflectionDir = normalize(reflect(ray.direction, N));
            // 反射起点，添加一个小的偏移量防止自相交
            Vector3f reflectionOrig = dotProduct(N, reflectionDir) >= 0 ? hitPoint + N * EPS : hitPoint - N * EPS;
            // 计算反射率
            float k = fresnel(ray.direction, N, m->ior);
            k = 0.8;
            // k = 0.01;
            hitColor = k * castRay(Ray(reflectionOrig, reflectionDir), depth + 1);
        } else {
            // 普通材质

            // 漫反射和镜面反射
            Vector3f lightAmt = 0, specularColor = 0;
            Vector3f shadowPointOrig = (dotProduct(ray.direction, N) < 0) ? hitPoint + N * EPS : hitPoint - N * EPS;
            for (auto pLight : light) {
                Vector3f lightdir = pLight->position - shadowPointOrig;
                float lightdist2 = dotProduct(lightdir, lightdir);
                lightdir = normalize(lightdir);
                // 计算法向量和指向光线的向量的夹角余弦值
                float cosi = std::max(0.0f, dotProduct(lightdir, N));
                auto shadow = this->intersection(Ray(shadowPointOrig, lightdir));
                bool inshadow = shadow.happened && shadow.distance * shadow.distance < lightdist2;
                if (!inshadow) {
                    // 不在阴影中
                    // 计算漫反射光
                    lightAmt += cosi * pLight->intensity;
                    // 计算高光
                    Vector3f toLight_reflect = normalize(reflect(-lightdir, N));
                    specularColor += std::pow(std::max(0.0f, dotProduct(toLight_reflect, -ray.direction)), m->specularExponent) * pLight->intensity;
                }
            }
            hitColor = lightAmt * (m->Kd * hitObject->evalDiffuseColor(st) + m->Ks * specularColor);
        }
    }
    return hitColor;
}