
#include "distortion.hpp"
#include <iostream>

bool Distortion::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {
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

    // Apply pincushion distortion
    float distorted_r = r_norm / (1 + k1 * r_norm * r_norm + k2 * r_norm * r_norm * r_norm * r_norm);

    // Calculate the distorted direction
    float ray_dir_x = (xc / r_norm) * distorted_r;
    float ray_dir_y = (yc / r_norm) * distorted_r;
    float ray_dir_z = 1.0f; // The ray points towards the scene along the Z axis

    Vector dirc = Vector(ray_dir_x, ray_dir_y, ray_dir_z);
    dirc.normalize(); // Ensure the direction vector is normalized

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
