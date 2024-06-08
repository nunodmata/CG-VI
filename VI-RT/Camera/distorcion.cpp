#include "distorcion.hpp"
#include <cmath>

BarrelDistortion::BarrelDistortion(Point Eye, Point At, const Vector Up, int W, int H, float strength)
    : strength(strength), W(W), H(H) {
    // Compute camera to world transform
    Vector f = Eye.vec2point(At);
    f.normalize();
    Vector r = f.cross(Up);
    r.normalize();
    Vector u = Up;
    
    c2w[0][0] = r.X;
    c2w[0][1] = r.Y;
    c2w[0][2] = r.Z;
    c2w[1][0] = u.X;
    c2w[1][1] = u.Y;
    c2w[1][2] = u.Z;
    c2w[2][0] = f.X;
    c2w[2][1] = f.Y;
    c2w[2][2] = f.Z;
}

bool BarrelDistortion::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {
    float xc, yc;
    if (cam_jitter == NULL) {
        xc = 2.f * ((float)x + .5f) / W - 1.f;
        yc = 2.f * ((float)(H - y - 1) + .5f) / H - 1.f;
    } else {
        xc = 2.f * ((float)x + cam_jitter[0]) / W - 1.f;
        yc = 2.f * ((float)(H - y - 1) + cam_jitter[1]) / H - 1.f;
    }

    float r_norm = sqrt(xc * xc + yc * yc);

    if (r_norm <= 1.0) {

    // Apply barrel distortion
    float theta = atan2(yc, xc);
    float r_distorted = r_norm * (1 + strength * r_norm * r_norm);
    
    float distorted_x = r_distorted * cos(theta);
    float distorted_y = r_distorted * sin(theta);

     Vector dir(distorted_x,distorted_y,-1);
     dir.normalize();


    //Transform the ray direction from camera space to world space
    Vector dir_cw;
    dir_cw.X = c2w[0][0] * r->dir.X + c2w[0][1] * r->dir.Y + c2w[0][2] * r->dir.Z;
    dir_cw.Y = c2w[1][0] * r->dir.X + c2w[1][1] * r->dir.Y + c2w[1][2] * r->dir.Z;
    dir_cw.Z = c2w[2][0] * r->dir.X + c2w[2][1] * r->dir.Y + c2w[2][2] * r->dir.Z;
    
    r->o=Point(0,0,0);
    r->dir = dir_cw; // Update the ray direction with the transformed direction

    return true;

    } else return false;
}
