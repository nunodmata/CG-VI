#include <iostream>
#include <fstream>
#include "ImagePFM.hpp"

void ImagePFM::ToneMap() {
    imageToSave = new PFM_pixel[W*H];

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

ImagePFM::ImagePFM(Image &img) {
    W = img.getW();
    H = img.getH();
    imagePlane = new RGB[W * H];
    imagePlane = img.getImagePlane();
}

bool ImagePFM::Save(std::string filename) {
   // Apply tone mapping to convert image data to PFM format
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

        //write header info to the file , negative value=>little endian
        ofs << "PF\n" << W << " " << H << "\n-1.0\n";

        // Write pixel data to the file
        //from bottom to top, from left to right, otherwise the image would be inverted.
        for (int j = this->H - 1; j >= 0; --j) {
        for (int i = 0; i < this->W; ++i) {
                float r = static_cast<float>(imageToSave[j * W + i].val[0]) / 255.0f;
                float g = static_cast<float>(imageToSave[j * W + i].val[1]) / 255.0f;
                float b = static_cast<float>(imageToSave[j * W + i].val[2]) / 255.0f;

                ofs.write(reinterpret_cast<char*>(&r), sizeof(float));
                ofs.write(reinterpret_cast<char*>(&g), sizeof(float));
                ofs.write(reinterpret_cast<char*>(&b), sizeof(float));
            }
        }

        ofs.close();
        return true;
    }
    catch (const char* err) {
        std::cerr << err << std::endl;
        ofs.close();
        return false;
    }
}

