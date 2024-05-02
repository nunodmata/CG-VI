//
//  main.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#include "Scene/scene.hpp"
#include "Camera/perspective.hpp"
#include "Renderer/StandardRenderer.hpp"
#include "Image/ImagePPM.hpp"
#include "Shader/AmbientShader.hpp"
#include "Shader/WhittedShader.hpp"
#include "Shader/DistributedShader.hpp"
#include "Shader/PathTracerShader.hpp"
#include "Light/AmbientLight.hpp"
#include "Light/PointLight.hpp"
#include "Light/AreaLight.hpp"

#include <filesystem> // Include the filesystem library for directory operations
#include <time.h>

int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective *cam; // Camera
    ImagePPM *img;    // Image
    Shader *shd;
    bool success;
    clock_t start, end;
    double cpu_time_used;
    
    //success = scene.Load("/home/nuno/VI/CG-VI/VI-RT/Scene/tinyobjloader/models/cornell_box.obj");

    success = scene.Load("../../VI-RT/Scene/tinyobjloader/models/cornell_box_VI.obj");
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }
    std::cout << "Scene Load: SUCCESS!! :-)\n";
    scene.printSummary();
    std::cout << std::endl;
    
    // add an ambient light to the scene
    AmbientLight ambient(RGB(0.25,0.25,0.25));
    scene.lights.push_back(&ambient);
    scene.numLights++;
    // add a point light to the scene
    /*PointLight *pl1 = new PointLight(RGB(0.65,0.65,0.65), Point(288,508,282));
    scene.lights.push_back(pl1);
    scene.numLights++;*/
    // add a area light to the scene
    AreaLight *al1 = new AreaLight(RGB(0.65,0.65,0.65), Point(288,548,282), Point(288,548,200), Point(203,548,282), Vector(0,-1,0));
    scene.lights.push_back(al1);
    scene.numLights++;
    AreaLight *al2 = new AreaLight(RGB(0.65,0.65,0.65), Point(203,548,200), Point(288,548,200), Point(203,548,282), Vector(0,-1,0));
    scene.lights.push_back(al2);
    scene.numLights++;



    // Image resolution
    const int W= 1024;
    const int H= 1024;
    
    img = new ImagePPM(W,H);
    
    // Camera parameters
    const Point Eye ={280,275,-330}, At={280,265,0};
    const Vector Up={0,1,0};
    const float fovW = 90.f;
    const float fovH = fovW * (float)H/(float)W;  // in degrees
    const float fovWrad = fovW*3.14f/180.f, fovHrad = fovH*3.14f/180.f;    // to radians
    cam = new Perspective(Eye, At, Up, W, H, fovWrad, fovHrad);
    
    // create the shader
    RGB background(0.05, 0.05, 0.55);

    //shd = new AmbientShader(&scene, background);

    //shd = new WhittedShader(&scene, background);

    shd = new DistributedShader(&scene, background);


    // declare the renderer
    int spp=16;     // samples per pixel
    StandardRenderer myRender (cam, &scene, img, shd, spp);
    // render
    start = clock();
    myRender.Render();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // save the image
    img->Save("MyImage.ppm");

    // Print the directory of the saved image
    std::cout << "Image saved to: " << std::filesystem::current_path() << std::endl;

    fprintf (stdout, "Rendering time = %.3lf secs\n\n", cpu_time_used);
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}
