#ifndef _MESHVOLUME_BOUNDS3D_H_
#define _MESHVOLUME_BOUNDS3D_H_

#include "common.h"
#include "vector3d.h"

namespace MeshVolume {

struct Bounds3d {
    Bounds3d()
        : posMin(INFTY, INFTY, INFTY)
        , posMax(-INFTY, -INFTY, -INFTY) {
    }
    
    Bounds3d(const Vector3d &posMin_, const Vector3d &posMax_)
        : posMin(posMin_)
        , posMax(posMax_) {
    }
    
    Bounds3d(const Bounds3d &b)
        : posMin(b.posMin)
        , posMax(b.posMax) {
    }
    
    Bounds3d & operator=(const Bounds3d &b) {
        this->posMin = b.posMin;
        this->posMax = b.posMax;
        return *this;
    }
    
    Bounds3d extend(const Vector3d &v) {
        posMin = posMin.minimum(v);
        posMax = posMax.maximum(v);
    }
    
    bool inside(const Vector3d &v) const {
        return (posMin.x <= v.x && v.x <= posMax.x) &&
               (posMin.y <= v.y && v.y <= posMax.y) &&
               (posMin.z <= v.z && v.z <= posMax.z);
    }
    
    double volume() const {
        Vector3d diff = posMax - posMin;
        return std::abs(diff.x * diff.y * diff.z);
    }
    
    Vector3d center() const {
        return 0.5 * (posMin + posMax);
    }
    
    Vector3d posMin, posMax;
};

}  // namespace MeshVolume

#endif  // _MESHVOLUME_BOUNDS3D_H_
