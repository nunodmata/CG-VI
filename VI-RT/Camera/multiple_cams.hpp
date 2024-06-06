#ifndef multiple_cams_hpp
#define multiple_cams_hpp

#include "camera.hpp"
#include "../Rays/ray.hpp"
#include "../utils/vector.hpp"

#include <vector>


class MultipleCams : public Camera {
public:
    MultipleCams();
    ~MultipleCams();
    void addCamera(Camera* cam);
    bool GenerateRay(const int x, const int y, Ray* r, const float* cam_jitter = NULL) override;
    void getResolution(int* _W, int* _H) override;

private:
    std::vector<Camera*> cameras;
    int currentCamIndex;
    int width, height;
};

#endif // MULTIPLE_CAMS_HPP