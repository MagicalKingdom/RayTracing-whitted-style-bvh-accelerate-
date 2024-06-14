#include "Rander.hpp"
#include "global.hpp"
void Rander::rander(Scene& scene) {
    std::vector<std::vector<Vector3f>> image(scene.height, std::vector<Vector3f>(scene.width));
    Vector3f eye_pos = scene.eye_pos;
    float scale = tan(deg2rad(scene.fov * 0.5));
    float imageAspectRatio = (float)scene.width / (float)scene.height;  // 宽高比
    for (size_t j = 0; j < scene.height; j += 1) {
        for (size_t i = 0; i < scene.width; i += 1) {
            float x = (2 * ((float)i + 0.5) / (float)scene.width - 1) * imageAspectRatio * scale;
            float y = (1 - 2 * ((float)j + 0.5) / (float)scene.height) * scale;
            if (j == scene.height / 2 && i == scene.width / 2) {
                int x = 2;
            }
            Vector3f raydir(x, y, -1);
            raydir = normalize(raydir);
            image[j][i] = scene.castRay(Ray(eye_pos, raydir), 0);
            std::clamp(image[j][i].x, 0.0f, 1.0f);
            std::clamp(image[j][i].y, 0.0f, 1.0f);
            std::clamp(image[j][i].z, 0.0f, 1.0f);
            image[j][i].x = std::pow(image[j][i].x, scene.gamma);
            image[j][i].y = std::pow(image[j][i].y, scene.gamma);
            image[j][i].z = std::pow(image[j][i].z, scene.gamma);
        }
    }

    FILE* fp = fopen("binary.ppm", "wb");
    // FILE* fp = fopen("binary.txt", "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", scene.width, scene.height);
    for (auto i = 0; i < scene.height; ++i) {
        for (auto j = 0; j < scene.width; j++) {
            static unsigned char color[3];
            color[0] = (unsigned char)(255 * std::clamp(image[i][j].x, 0.0f, 1.0f));
            color[1] = (unsigned char)(255 * std::clamp(image[i][j].y, 0.0f, 1.0f));
            color[2] = (unsigned char)(255 * std::clamp(image[i][j].z, 0.0f, 1.0f));
            fwrite(color, 1, 3, fp);
            //(void)fprintf(fp, "(%d,%d) %d %d %d\n", i, j, color[0], color[1], color[2]);
        }
    }
    fclose(fp);
}