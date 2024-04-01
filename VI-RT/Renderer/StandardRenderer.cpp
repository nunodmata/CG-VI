//
//  StandardRenderer.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "StandardRenderer.hpp"

void StandardRenderer::Render () {
    int W=0, H=0;  // resolution
    int x,y;

    // get resolution from the camera
    this->cam->getResolution(&W, &H);
    
    // main rendering loop: get primary rays from the camera until done
    for (y=0 ; y< H ; y++) {  // loop over rows
        for (x=0 ; x< W ; x++) { // loop over columns
            Ray primary;
            Intersection isect;
            bool intersected;
            RGB color;
          
            
            for (int ss = 0; ss < spp; ss++)
            {
                
                float jitterV[2] = {(float)rand() / RAND_MAX, (float)rand() / RAND_MAX};
                // Generate Ray (camera)
                this->cam->GenerateRay(x, y, &primary, jitterV);
                
                // trace ray (scene)
                intersected = scene->trace(primary, &isect);

                // shade this intersection (shader) - remember: depth=0
                RGB this_color = shd->shade(intersected, isect, 0);
                color += this_color;
            }

            // write the result into the image frame buffer (image)
            color = color * (1.f / spp);
            img->set(x,y,color);
            
        } // loop over columns
    }   // loop over rows
}
