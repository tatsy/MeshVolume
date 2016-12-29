#ifndef _MESHVOLUME_TRIANGLE_H_
#define _MESHVOLUME_TRIANGLE_H_

#include "vector3d.h"

namespace MeshVolume {

struct Triangle {
    Triangle()
        : v0()
        , v1()
        , v2() {
    }
    
    Triangle(const Vector3d &v0_, const Vector3d &v1_, const Vector3d &v2_)
        : v0(v0_)
        , v1(v1_)
        , v2(v2_) {
    }
    
    Triangle(const Triangle &t)
        : v0(t.v0)
        , v1(t.v1)
        , v2(t.v2) {
    }
    
    Triangle & operator=(const Triangle &t) {
        this->v0 = t.v0;
        this->v1 = t.v1;
        this->v2 = t.v2;
        return *this;
    }
    
    Vector3d v0, v1, v2;
};

}  // namespace MeshVolume

#endif  // _MESHVOLUME_TRIANGLE_H_
