#include "ImagePPM.hpp"
#include "Uncharted2Tonemap.hpp"
#include <iostream>
#include <fstream>

void ImagePPM::ToneMap() {
    imageToSave = new PPM_pixel[W*H];

    // loop over each pixel in the image and tone map
    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; ++i) {
            // Retrieve the original color
            RGB color = imagePlane[j*W + i];

            // Apply Uncharted 2 tone mapping
            RGB toneMappedColor = Uncharted2Tonemap(color);

            // Convert to 8-bit color and store in imageToSave
            imageToSave[j*W + i].val[0] = static_cast<unsigned char>(std::min(1.0f, std::max(0.0f, toneMappedColor.R)) * 255.0f);
            imageToSave[j*W + i].val[1] = static_cast<unsigned char>(std::min(1.0f, std::max(0.0f, toneMappedColor.G)) * 255.0f);
            imageToSave[j*W + i].val[2] = static_cast<unsigned char>(std::min(1.0f, std::max(0.0f, toneMappedColor.B)) * 255.0f);
        }
    }
}

ImagePPM::ImagePPM(Image &img) {
    W = img.getW();
    H = img.getH();
    imagePlane = new RGB[W*H];
    imagePlane = img.getImagePlane();
}

bool ImagePPM::Save(std::string filename) {
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
