#ifndef _MESHVOLUME_COMMON_H_
#define _MESHVOLUME_COMMON_H_

namespace MeshVolume {

static const double INFTY = 1.0e20;
static const double EPS = 1.0e-12;

enum Relation : int {
    Inside = 0x00,
    Outside = 0x01,
    Intersect = 0x02,
    Included = 0x03
};
    
}  // namespace MeshVolume

#endif  // _MESHVOLUME_COMMON_H_
