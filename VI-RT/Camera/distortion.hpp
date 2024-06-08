//
//  distortion.hpp
//  VI-RT
//

#ifndef distortion_hpp
#define distortion_hpp

#include "camera.hpp"
#include "../Rays/ray.hpp"
#include "../utils/vector.hpp"

class Distortion : public Camera {
    Point Eye, At;
    Vector Up;
    float fovW, fovH;
    int W, H;
    float c2w[3][3];  // camera to world transform
public:
    Distortion(Point Eye, Point At, const Vector Up, const int W, const int H, const float fovW, const float fovH)
        : Eye(Eye), At(At), Up(Up), W(W), H(H), fovW(fovW), fovH(fovH) {
        // compute camera to world transform
        Vector f = Eye.vec2point(At);
        f.normalize();
        Vector r = f.cross(Up);
        r.normalize();
        Vector u = Up;
        // Create matrix c2w
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
    bool GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter = NULL);
    void getResolution(int *_W, int *_H) { *_W = W; *_H = H; }
};

#endif /* distortion_hpp */