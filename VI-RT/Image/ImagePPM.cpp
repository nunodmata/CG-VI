//
//  ImagePPM.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#include "ImagePPM.hpp"
#include <iostream>
#include <fstream>

void ImagePPM::ToneMap() {
    imageToSave = new PPM_pixel[W*H];

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
bool ImagePPM::Save (std::string filename) {
    
    // convert from float to {0,1,..., 255}
    ToneMap();
    printf("ToneMap\n");

    // write imageToSave to file
    if (this->W == 0 || this->H == 0) {
        fprintf(stderr, "Can't save an empty image\n");
        return false;
    }

    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary);
        if (ofs.fail()) throw("Can't open output file");

        ofs << "P6\n" << this->W << " " << this->H << "\n255\n";
        unsigned char r, g, b;

        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i < this->W * this->H; ++i) {
            r = imageToSave[i].val[0];
            g = imageToSave[i].val[1];
            b = imageToSave[i].val[2];
            ofs << r << g << b;
        }
        ofs.close();
        return true;
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
        return false;
    }

}

//Save image as PFM
bool ImagePPM::ToPFM(std::string filename){
 std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary);
        if (ofs.fail()) throw("Can't open output file");

        // Write the header information to file 
        //PF=> Format , -1.0 =>scale factor
        ofs << "PF\n" << this->W << " " << this->H << "\n-1.0\n";

        // Write the pixel data
        for (int j = 0; j < this->H; ++j) {
            for (int i = 0; i < this->W; ++i) {
                // Convert RGB values to floats and write them to the file
               // Pixel values are normalized to the range [0, 1]
                float r = static_cast<float>(imagePlane[j * W + i].R) / 255.0f;
                float g = static_cast<float>(imagePlane[j * W + i].G) / 255.0f;
                float b = static_cast<float>(imagePlane[j * W + i].B) / 255.0f;

                ofs.write(reinterpret_cast<char*>(&r), sizeof(float));
                ofs.write(reinterpret_cast<char*>(&g), sizeof(float));
                ofs.write(reinterpret_cast<char*>(&b), sizeof(float));
            }
        }

        ofs.close();
        return true;
    }
    catch (const char *err) {
        std::cerr << err << std::endl;
        ofs.close();
        return false;
    }
}
