#ifndef ImageEXR_hpp
#define ImageEXR_hpp

#include "image.hpp"
#include "Uncharted2Tonemap.hpp"
#include <opencv2/opencv.hpp>

class ImageEXR : public Image {
public:
    ImageEXR(const int W, const int H): Image(W, H) {}
    ImageEXR(Image &img);

    virtual bool Save(std::string filename) override;

private:
    void ToneMap(cv::Mat &hdr_image);
};

#endif
