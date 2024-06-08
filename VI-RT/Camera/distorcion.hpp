#ifndef DISTORCION_HPP
#define DISTORCION_HPP

#include "camera.hpp"

class BarrelDistortion : public Camera {
    float strength;
    int W, H;
    float c2w[3][3];  // camera to world transform
public:
    BarrelDistortion(Point Eye, Point At, const Vector Up, int W, int H, float strength);
    bool GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter=NULL);
    void getResolution(int *_W, int *_H) {*_W = W; *_H = H;}
};

#endif // BARREL_DISTORTION_HPP
