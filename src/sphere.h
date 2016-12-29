#ifndef _MESHVOLUME_SPHERE_H_
#define _MESHVOLUME_SPHERE_H_

#include "vector3d.h"
#include "bounds3d.h"

namespace MeshVolume {

struct Sphere {
    Sphere()
        : c()
        , r(0.0) {
    }

    Sphere(const Vector3d &c_, double r_)
        : c(c_)
        , r(r_) {
    }
    
    Sphere(const Sphere &s)
        : c(s.c)
        , r(s.r) {
    }
    
    Sphere & operator=(const Sphere &s) {
        this->c = s.c;
        this->r = s.r;
        return *this;
    }
    
    bool inside(const Vector3d &p) const {
        return (this->c - p).lengthSquared() < (this->r * this->r);
    }
    
    int relation(const Bounds3d &b) const {
        bool isInside = true;
        bool isOutside = true;
        for (int i = 0; i < 8; i++) {
            double vx = ((i >> 0) & 0x01) == 0 ? b.posMin.x : b.posMax.x;
            double vy = ((i >> 1) & 0x01) == 0 ? b.posMin.y : b.posMax.y;
            double vz = ((i >> 2) & 0x01) == 0 ? b.posMin.z : b.posMax.z;
            Vector3d v(vx, vy, vz);
            if (inside(v)) {
                isOutside = false;
            } else {
                isInside = false;
            }
        }
        
        if (isInside) return (int)Relation::Inside;
        if (isOutside) {
            if (!b.inside(c)) {
                return (int)Relation::Outside;
            } else {
                Vector3d dMin = b.posMin - c;
                Vector3d dMax = b.posMax - c;
                if ((std::abs(dMin.x) < r || std::abs(dMax.x) < r) ||
                    (std::abs(dMin.y) < r || std::abs(dMax.y) < r) ||
                    (std::abs(dMin.z) < r || std::abs(dMax.z) < r)) {
                    return (int)Relation::Intersect;
                } else {
                    return (int)Relation::Included;
                }
            }
        }
        
        return (int)Relation::Intersect;
    }
    
    Vector3d c;
    double r;
};
    
}  // namespace MeshVolume

#endif  // _MESHVOLUME_SPHERE_H_
