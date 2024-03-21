//
//  Scene.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <iostream>
#include <string>
#include <vector>
#include "primitive.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "intersection.hpp"
#include "triangle.hpp"
#include "BRDF.hpp"

typedef struct rehash { //PODE TER ERRO
    int objNdx;   // Index of the vertex in the original mesh
    int ourNdx;   // Index of the vertex in our representation
    // Constructor to initialize objNdx and ourNdx
    rehash(int objIndex, int ourIndex) : objNdx(objIndex), ourNdx(ourIndex) {}
    // Comparison operator for set operations
    bool operator<(const rehash& other) const {
        return objNdx < other.objNdx;
    }
}rehash;


class Scene {
    std::vector <Primitive *> prims;
    std::vector <BRDF *> BRDFs;
public:
    std::vector <Light *> lights;
    int numPrimitives, numLights, numBRDFs;

    Scene (): numPrimitives(0), numLights(0), numBRDFs(0) {}
    bool Load (const std::string &fname);
    bool SetLights (void) { return true; };
    bool trace (Ray r, Intersection *isect);
    bool visibility (Ray s, const float maxL);
    void printSummary(void) {
        std::cout << "#primitives = " << numPrimitives << " ; ";
        std::cout << "#lights = " << numLights << " ; ";
        std::cout << "#materials = " << numBRDFs << " ;" << std::endl;
    }
};

#endif /* Scene_hpp */
