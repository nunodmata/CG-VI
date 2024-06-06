#include "multiple_cams.hpp"
#include <iostream>
#include <cmath>

// Constructor
MultipleCams::MultipleCams() : currentCamIndex(0), width(0), height(0) {}

// Destructor
MultipleCams::~MultipleCams() {
    for (Camera* cam : cameras) {
        delete cam;
    }
}

// Add a camera to the list
void MultipleCams::addCamera(Camera* cam) {
    cameras.push_back(cam);
    int camWidth, camHeight;
    cam->getResolution(&camWidth, &camHeight);
    if (width == 0 && height == 0) {
        width = camWidth * 2;
        height = camHeight * 2;
    }
}

// Generate a ray from the current camera
bool MultipleCams::GenerateRay(const int x, const int y, Ray* r, const float* cam_jitter) {
    if (cameras.empty()) return false;

    int numCams = cameras.size();
    int cols = 2;
    int rows = 2;
    int subWidth = width / cols;
    int subHeight = height / rows;

    int camX = x / subWidth;
    int camY = y / subHeight;

    int camIndex = camY * cols + camX;
    if (camIndex >= numCams) return false;

    int localX = x % subWidth;
    int localY = y % subHeight;

    bool result = cameras[camIndex]->GenerateRay(localX, localY, r, cam_jitter);
    currentCamIndex = (currentCamIndex + 1) % numCams;
    return result;
}

// Get the resolution of the current camera
void MultipleCams::getResolution(int* _W, int* _H) {
    *_W = width;
    *_H = height;
}