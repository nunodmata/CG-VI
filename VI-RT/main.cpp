//
//  main.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#include "Scene/scene.hpp"
#include "Camera/perspective.hpp"
#include "Camera/fish_eye.hpp"
#include "Camera/multiple_cams.hpp"
#include "Camera/distorcion.hpp"
#include "Renderer/StandardRenderer.hpp"
#include "Image/ImagePPM.hpp"
#include "Image/ImageJPG.hpp"
#include "Image/ImageEXR.hpp"
#include "Image/ImagePFM.hpp"
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
    if(argc != 4){
        std::cout << "Usage: " << argv[0] << " <Camera_Type> <format> <output_file>" << std::endl;
        std::cout << "Camera_Type: FISH_EYE(fish_eye) MULTIPLE_CAMS(multiple_cams) placeholder" << std::endl;
        std::cout << "Format: PPM(ppm), PFM(pfm), JPG(jpg), OPENEXR(openexr)" << std::endl;
        std::cout << "Output file should have extension according to the selected format" << std::endl;
        return 1;
    }
    else{
        std::cout << "Selected Camera Type: " << argv[1] << std::endl;
        std::cout << "Selected Format: " << argv[2] << std::endl;
        std::cout << "Selected Output File: " << argv[3] << std::endl;
    }

    Scene scene;
    Image *img;    // Image
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
    /*AmbientLight ambient(RGB(0.25,0.25,0.25));
    scene.lights.push_back(&ambient);
    scene.numLights++;*/
    // add a point light to the scene
    /*PointLight *pl1 = new PointLight(RGB(0.65,0.65,0.65), Point(288,508,282));
    scene.lights.push_back(pl1);
    scene.numLights++;*/
    // add a area light to the scene
    AreaLight *al1 = new AreaLight(RGB(1,1,1), Point(347.5, 548, 349.2), Point(208.5, 548, 349.2), Point(208.5, 548, 209.2), Vector(0,-1,0));
    scene.lights.push_back(al1);
    scene.numLights++;
    AreaLight *al2 = new AreaLight(RGB(1,1,1), Point(347.5, 548, 349.2), Point(208.5, 548, 209.2), Point(347.5, 548, 209.2), Vector(0,-1,0));
    scene.lights.push_back(al2);
    scene.numLights++;



    // Image resolution
    const int W= 1024;
    const int H= 1024;
    
    img = new Image(W,H);
    
    // Camera parameters
    const Point Eye ={280,275,-330}, At={280,265,0};
    const Vector Up={0,1,0};
    const float fovW = 90.f;
    const float fovH = fovW * (float)H/(float)W;  // in degrees
    const float fovWrad = fovW*3.14f/180.f, fovHrad = fovH*3.14f/180.f;    // to radians

    Camera *cam;
    std::string CameraType(argv[1]);
    if(CameraType == "FISH_EYE" || CameraType == "fish_eye"){
        cam = new Fish_Eye(Eye, At, Up, W, H, fovWrad, fovHrad);
    }
    else if (CameraType == "MULTIPLE_CAMS" || CameraType == "multiple_cams") {
        MultipleCams* multiCam = new MultipleCams();
        // we can also add the fish eye effect to one of the cameras in the multiple camera setup like the commented lines below!!!
        //multiCam->addCamera(new Fish_Eye(Eye, At, Up, W / 2, H / 2, fovWrad, fovHrad)); //1st image top left
        multiCam->addCamera(new Perspective(Eye, At, Up, W / 2, H / 2, fovWrad, fovHrad)); //1st image top left
        multiCam->addCamera(new Perspective(Point(0,400,0), Point(35,450,80), Up, W, H / 2, fovWrad, fovHrad)); //2nd image top right
       
        //multiCam->addCamera(new Fish_Eye(Point(200,205,-330), At, Up, W / 2, H / 2, fovWrad, fovHrad)); //3rd image bottom left
        multiCam->addCamera(new Perspective(Point(250,475,550), Point(420,-70,0), Up, W / 2, H / 2, fovWrad, fovHrad)); //3rd image bottom left
        multiCam->addCamera(new Perspective(Point(213,425,200), Point(250,30,20), Up, W / 2, H / 2, fovWrad, fovHrad)); //4th image bottom right
        cam = multiCam;
        
    } else if (CameraType == "BARREL_DISTORTION" || CameraType == "barrel_distortion") {
        cam = new BarrelDistortion(Eye, At, Up, W, H, 0.5);
}else {
        cam = new Perspective(Eye, At, Up, W, H, fovWrad, fovHrad);
    }
    
    
    // create the shader
    RGB background(0.05, 0.05, 0.55);

    //shd = new AmbientShader(&scene, background);

    //shd = new WhittedShader(&scene, background);

    //shd = new DistributedShader(&scene, background);

    shd = new PathTracerShader(&scene, background);


    // declare the renderer
    int spp=32;     // samples per pixel
    StandardRenderer myRender (cam, &scene, img, shd, spp);
    // render
    start = clock();
    myRender.Render();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

// Convert the image to PFM format
    std::string format(argv[2]);
    std::string FileName(argv[3]);

    if(format == "PPM" || format == "ppm") { //PPM
    
        ImagePPM imgPPM = ImagePPM(*img);
        // save the image
        if(!imgPPM.Save(FileName)){
            std::cerr << "Error converting image to PPM format." << std::endl;
            return 1;
        }
        std::cout << "Image converted to PPM format: " << FileName << std::endl;
            
        
    }
    else if(format == "PFM" || format == "pfm") { //PFM
        ImagePFM imgPFM = ImagePFM(*img);
        // save the image
        if(!imgPFM.Save(FileName)){
            std::cerr << "Error converting image to PFM format." << std::endl;
            return 1;
        }
        
        std::cout << "Image converted to PFM format: " << FileName << std::endl;
    }
    else if(format == "JPG" || format == "jpg"){ //JPG
        ImageJPG imgJPG = ImageJPG(*img);
        // save the image
        if(!imgJPG.Save(FileName)){
            std::cerr << "Error converting image to JPG format." << std::endl;
            return 1;
        }
        std::cout << "Image converted to JPG format: " << FileName << std::endl;
    }
    else if(format == "OPENEXR" || format == "openexr"){
        ImageEXR imgEXR = ImageEXR(*img);
        
        if(!imgEXR.Save(FileName)){
            std::cerr << "Error converting image to OPENEXR format." << std::endl;
            return 1;
        }
         //OPENEXR
        std::cerr << "Image saved in OPENEXR format: " << FileName << std::endl;
        return 1;
    }
    else{
        std::cout << "Invalid format" << std::endl;
    }

    // Print the directory of the saved image
    std::cout << "Image saved to: " << std::filesystem::current_path() << std::endl;

    fprintf (stdout, "Rendering time = %.3lf secs\n\n", cpu_time_used);
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}
