//
//  AABB.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef BB_hpp
#define BB_hpp

#include "vector.hpp"
#include "ray.hpp"

typedef struct BB {
public:
    Point min, max;
    void update (Point p) {
        if (p.X < min.X) min.X = p.X;
        else if (p.X > max.X) max.X = p.X;
        if (p.Y < min.Y) min.Y = p.Y;
        else if (p.Y > max.Y) max.Y = p.Y;
        if (p.Z < min.Z) min.Z = p.Z;
        else if (p.Z > max.Z) max.Z = p.Z;
    }
    /*
     * I suggest you implement:
     *  bool intersect (Ray r) { }
     *
     * based on PBRT's 3rd ed. book , sec 3.1.2, pags 125..128 + 214,217,221
     *
     * or https://doi.org/10.1007/978-1-4842-7185-8_2
     *
     */
    bool intersect (Ray r) { 
        
        float tmin = (min.X - r.o.X) / r.dir.X;
        float tmax = (max.X - r.o.X) / r.dir.X;
        if (tmin > tmax) std::swap(tmin, tmax);

        float tymin = (min.Y - r.o.Y) / r.dir.Y;
        float tymax = (max.Y - r.o.Y) / r.dir.Y;
        if (tymin > tymax) std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax)) return false;

        if (tymin > tmin) tmin = tymin;
        if (tymax < tmax) tmax = tymax;

        float tzmin = (min.Z - r.o.Z) / r.dir.Z;
        float tzmax = (max.Z - r.o.Z) / r.dir.Z;
        if (tzmin > tzmax) std::swap(tzmin, tzmax);


        if ((tmin > tzmax) || (tzmin > tmax)) return false;    
        
        
        return true; }
} BB;

#endif /* AABB_hpp */
