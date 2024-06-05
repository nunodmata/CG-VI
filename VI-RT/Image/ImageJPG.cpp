//
//  ImagePPM.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#include "ImageJPG.hpp"
#include <iostream>
#include <fstream>

void ImageJPG::ToneMap() {
    imageToSave = new JPG_pixel[W*H];

    // loop over each pixel in the image, calculate luminance, and tone map
    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; ++i) {
            // Calculate luminance from RGB values (using approximate luminance weights)
            float luminance = 0.2126f * imagePlane[j*W+i].R + 0.7152f * imagePlane[j*W+i].G + 0.0722f * imagePlane[j*W+i].B;

            // Apply Reinhard tone mapping operator
            float toneMappedLuminance = luminance / (1.0f + luminance);

            // Convert back to RGB
            imageToSave[j*W+i].val[0] = (unsigned char)(toneMappedLuminance * imagePlane[j*W+i].R / luminance * 255);
            imageToSave[j*W+i].val[1] = (unsigned char)(toneMappedLuminance * imagePlane[j*W+i].G / luminance * 255);
            imageToSave[j*W+i].val[2] = (unsigned char)(toneMappedLuminance * imagePlane[j*W+i].B / luminance * 255);
        }
    }
}

ImageJPG::ImageJPG(Image &img){
    W = img.getW();
    H = img.getH();
    imagePlane = new RGB[W*H];
    imagePlane = img.getImagePlane();
}

bool ImageJPG::Save (std::string filename) {
    
    // convert from float to {0,1,..., 255}
    ToneMap();
    printf("ToneMap\n");

    // write imageToSave to file
    if (this->W == 0 || this->H == 0) {
        fprintf(stderr, "Can't save an empty image\n");
        return false;
    }

    try {
        cv::Mat imageJPG(H, W, CV_8UC3, cv::Scalar(0,0,0)); //CV_8UC3 -> 8-bit unsigned, 3 channels
        for (int j = 0; j < H; j++) {
            for (int i = 0; i < W; ++i) {
                imageJPG.at<cv::Vec3b>(j, i)[0] = imageToSave[j*W+i].val[2];
                imageJPG.at<cv::Vec3b>(j, i)[1] = imageToSave[j*W+i].val[1];
                imageJPG.at<cv::Vec3b>(j, i)[2] = imageToSave[j*W+i].val[0];
            }
        }

        cv::imwrite(filename, imageJPG);
        return true;
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        return false;
    }

}
