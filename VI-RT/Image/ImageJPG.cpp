//
//  ImagePPM.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#include "ImageJPG.hpp"
#include "Uncharted2Tonemap.hpp"
#include <iostream>
#include <fstream>

void ImageJPG::ToneMap() {
    imageToSave = new JPG_pixel[W*H];

    // loop over each pixel in the image, calculate luminance, and tone map
    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; ++i) {
            // Retrieve the original color
            RGB color = imagePlane[j*W + i];

            // Apply Uncharted 2 tone mapping
            RGB toneMappedColor = Uncharted2Tonemap(color);

            // Convert back to RGB
            imageToSave[j*W + i].val[0] = static_cast<unsigned char>(std::min(1.0f, std::max(0.0f, toneMappedColor.R)) * 255.0f);
            imageToSave[j*W + i].val[1] = static_cast<unsigned char>(std::min(1.0f, std::max(0.0f, toneMappedColor.G)) * 255.0f);
            imageToSave[j*W + i].val[2] = static_cast<unsigned char>(std::min(1.0f, std::max(0.0f, toneMappedColor.B)) * 255.0f);
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
