//
//  perspective.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#include "fish_eye.hpp"
#include <iostream>

bool Fish_Eye::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {
    //Camera Space 
    float xc, yc;
    if (cam_jitter == NULL) {
        xc = 2.f * ((float)x + .5f) / W - 1.f;
        yc = 2.f * ((float)(H - y - 1) + .5f) / H - 1.f;
    }
    else {
        xc = 2.f * ((float)x + cam_jitter[0]) / W - 1.f;
        yc = 2.f * ((float)(H - y - 1) + cam_jitter[1]) / H - 1.f;
    }

    float r_norm = sqrt(xc*xc + yc*yc);

    if (r_norm > 1.0f) {
        return false;
    }

    // Calculate the angle of the ray
    float radius = r_norm * fovW * 0.5f;    
    float distorted_radius = tan(radius);

    
    // Calculate the distorted direction
    float ray_dir_x = sin(distorted_radius) * xc;
    float ray_dir_y = sin(distorted_radius) * yc;
    float ray_dir_z = cos(distorted_radius);

    Vector dirc=Vector(ray_dir_x,ray_dir_y,ray_dir_z);

    //Camera Setup
    Vector* dir_cw=new Vector();
    //ray*dir
    dir_cw->X = c2w[0][0] * dirc.X + c2w[0][1] * dirc.Y + c2w[0][2] * dirc.Z;
    dir_cw->Y = c2w[1][0] * dirc.X + c2w[1][1] * dirc.Y + c2w[1][2] * dirc.Z;
    dir_cw->Z = c2w[2][0] * dirc.X + c2w[2][1] * dirc.Y + c2w[2][2] * dirc.Z;
    
    // set origin and direction of the ray
    r->o=Eye;
    r->dir=*dir_cw;
    return true;
}
