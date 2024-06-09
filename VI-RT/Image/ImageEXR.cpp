#include "ImageEXR.hpp"

void ImageEXR::ToneMap(cv::Mat &hdr_image) {
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            // Extract pixel
            cv::Vec3f &pixel = hdr_image.at<cv::Vec3f>(i, j);
            float R = pixel[2];
            float G = pixel[1];
            float B = pixel[0];

            // Apply Uncharted 2 tone mapping
            RGB color(R, G, B);
            RGB toneMappedColor = Uncharted2Tonemap(color);

            // Update the pixel with tone mapped values
            pixel[2] = toneMappedColor.R;
            pixel[1] = toneMappedColor.G;
            pixel[0] = toneMappedColor.B;
        }
    }
}

ImageEXR::ImageEXR(Image &img) {
    W = img.getW();
    H = img.getH();
    imagePlane = new RGB[W*H];
    imagePlane = img.getImagePlane();
}

//
// if you want to save the image without tone mapping
//
// bool ImageEXR::Save(std::string filename) {
//     cv::Mat hdr_image(H, W, CV_32FC3);

//     for (int i = 0; i < H; i++)
//         for (int j = 0; j < W; j++) {
//             RGB &rgb = imagePlane[i*W+j];
//             hdr_image.at<cv::Vec3f>(i, j) = cv::Vec3f(rgb.B, rgb.G, rgb.R);
//         }

//     std::vector<int> exr_params;
//     exr_params.push_back(cv::IMWRITE_EXR_TYPE);
//     exr_params.push_back(cv::IMWRITE_EXR_TYPE_FLOAT); //specify the datatype of a pixel. Here it is set as float

//     cv::imwrite(filename, hdr_image, exr_params);
//     return true;
// }

bool ImageEXR::Save(std::string filename) {
    cv::Mat hdr_image(H, W, CV_32FC3);

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            RGB &rgb = imagePlane[i * W + j];
            hdr_image.at<cv::Vec3f>(i, j) = cv::Vec3f(rgb.B, rgb.G, rgb.R);
        }
    }

    // Apply tone mapping
    ToneMap(hdr_image);
    printf("ToneMap\n");

    std::vector<int> exr_params;
    exr_params.push_back(cv::IMWRITE_EXR_TYPE);
    exr_params.push_back(cv::IMWRITE_EXR_TYPE_HALF); // OpenCV supports HALF and FLOAT, here it's set as half (unlike the previous Save function to use FLOAT)

    cv::imwrite(filename, hdr_image, exr_params);
    return true;
}
