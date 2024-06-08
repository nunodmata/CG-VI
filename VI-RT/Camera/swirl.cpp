#include "swirl.hpp"
#include <iostream>
#include <cmath>

// Helper function to compute the swirl effect
void swirlEffect(float &xc, float &yc, float amount) {
    float radius = sqrt(xc * xc + yc * yc);
    float angle = atan2(yc, xc) + radius * amount;
    xc = radius * cos(angle);
    yc = radius * sin(angle);
}

bool Swirl::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {
    // Camera Space
    float xc, yc;
    if (cam_jitter == NULL) {
        xc = 2.f * ((float)x + .5f) / W - 1.f;
        yc = 2.f * ((float)(H - y - 1) + .5f) / H - 1.f;
    } else {
        xc = 2.f * ((float)x + cam_jitter[0]) / W - 1.f;
        yc = 2.f * ((float)(H - y - 1) + cam_jitter[1]) / H - 1.f;
    }

    float r_norm = sqrt(xc * xc + yc * yc);
    if (r_norm > 1.0f) {
        return false;
    }

    // Apply swirl effect
    float swirl_amount = 6.0f; // Adjust this value for more or less swirl
    swirlEffect(xc, yc, swirl_amount);

    // Calculate the direction
    float ray_dir_x = xc;
    float ray_dir_y = yc;
    float ray_dir_z = sqrt(1.0f - ray_dir_x * ray_dir_x - ray_dir_y * ray_dir_y);

    Vector dirc = Vector(ray_dir_x, ray_dir_y, ray_dir_z);

    // Camera to World
    Vector dir_cw;
    dir_cw.X = c2w[0][0] * dirc.X + c2w[0][1] * dirc.Y + c2w[0][2] * dirc.Z;
    dir_cw.Y = c2w[1][0] * dirc.X + c2w[1][1] * dirc.Y + c2w[1][2] * dirc.Z;
    dir_cw.Z = c2w[2][0] * dirc.X + c2w[2][1] * dirc.Y + c2w[2][2] * dirc.Z;

    // Set origin and direction of the ray
    r->o = Eye;
    r->dir = dir_cw;
    return true;
}