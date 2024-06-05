#ifndef ImagePFM_hpp
#define ImagePFM_hpp
#include "image.hpp"
#include <opencv2/opencv.hpp>

class ImagePFM: public Image {
    typedef struct {
        unsigned char val[3];  // r,g,b
    } PFM_pixel;
    PFM_pixel *imageToSave;
    void ToneMap ();
public:
    ImagePFM(const int W, const int H):Image(W, H) {}
    ImagePFM(Image &img);
    bool Save (std::string filename);
};

#endif /* ImagePPM_hpp */