#ifndef _MESHVOLUME_BOUNDS2D_H_
#define _MESHVOLUME_BOUNDS2D_H_

#include "common.h"
#include "vector2d.h"

namespace MeshVolume {

struct Bounds2d {
    Bounds2d()
        : posMin(INFTY, INFTY)
        , posMax(-INFTY, -INFTY) {
    }
    
    Bounds2d(const Vector2d &posMin_, const Vector2d &posMax_)
        : posMin(posMin_)
        , posMax(posMax_) {
    }
    
    Bounds2d(const Bounds2d &b)
        : posMin(b.posMin)
        , posMax(b.posMax) {
    }
    
    Bounds2d & operator=(const Bounds2d &b) {
        this->posMin = b.posMin;
        this->posMax = b.posMax;
        return *this;
    }
    
    void extend(const Vector2d &v) {
        posMin = posMin.minimum(v);
        posMax = posMax.maximum(v);
    }
    
    void extend(const Bounds2d &b) {
        posMin = posMin.minimum(b.posMin);
        posMax = posMax.maximum(b.posMax);
    }
    
    double maxExtent() const {
        Vector2d diff = posMax - posMin;
        return std::max(diff.x, diff.y);
    }

    Vector2d center() const {
        return 0.5 * (posMin + posMax);
    }
    
    bool inside(const Vector2d &v) const {
        return (posMin.x <= v.x && v.x <= posMax.x) &&
               (posMin.y <= v.y && v.y <= posMax.y);
    }
    
    static Bounds2d merge(const Bounds2d &b1, const Bounds2d &b2) {
        Vector2d posMin = b1.posMin.minimum(b2.posMin);
        Vector2d posMax = b1.posMax.maximum(b2.posMax);
        return Bounds2d(posMin, posMax);
    }
    
    Vector2d posMin, posMax;
};

}  // namespace MeshVolume

#endif  // _MESHVOLUME_BOUNDS2D_H_
