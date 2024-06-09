//
//  ImagePPM_original.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#ifndef ImagePPM_original_hpp
#define ImagePPM_original_hpp
#include "image.hpp"

class ImagePPM_original: public Image {
    typedef struct {
        unsigned char val[3];  // r,g,b
    } PPM_pixel;
    PPM_pixel *imageToSave;
    void ToneMap ();
public:
    ImagePPM_original(const int W, const int H):Image(W, H) {}
    ImagePPM_original(Image &img);
    bool Save (std::string filename);
};

#endif /* ImagePPM_original_hpp */
