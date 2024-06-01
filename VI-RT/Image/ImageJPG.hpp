//
//  ImagePPM.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#ifndef ImageJPG_hpp
#define ImageJPG_hpp
#include "image.hpp"
#include <opencv2/opencv.hpp>

class ImageJPG: public Image {
    typedef struct {
        unsigned char val[3];  // r,g,b
    } JPG_pixel;
    JPG_pixel *imageToSave;
    void ToneMap ();
public:
    ImageJPG(const int W, const int H):Image(W, H) {}
    ImageJPG(Image &img);
    bool Save (std::string filename);
};

#endif /* ImagePPM_hpp */
