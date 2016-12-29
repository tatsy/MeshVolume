#ifndef _MESHVOLUME_VECTOR3D_H_
#define _MESHVOLUME_VECTOR3D_H_

#include <cmath>
#include <string>
#include <sstream>

namespace MeshVolume {

struct Vector3d {
    explicit Vector3d(double x_ = 0.0, double y_ = 0.0, double z_ = 0.0)
        : x(x_)
        , y(y_)
        , z(z_) {
    }
    
    Vector3d(const Vector3d &v)
        : x(v.x)
        , y(v.y)
        , z(v.z) {
    }
    
    Vector3d & operator=(const Vector3d &v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        return *this;
    }
    
    Vector3d operator-() const {
        return Vector3d(-x, -y, -z);
    }
    
    Vector3d operator+=(const Vector3d &v) {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
        return *this;
    }

    Vector3d operator-=(const Vector3d &v) {
        this->x -= v.x;
        this->y -= v.y;
        this->z -= v.z;
        return *this;
    }
    
    Vector3d operator*=(double s) {
        this->x *= s;
        this->y *= s;
        this->z *= s;
        return *this;
    }
    
    double dot(const Vector3d & v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    
    double length() const {
        return std::sqrt(this->lengthSquared());
    }
    
    double lengthSquared() const {
        return this->dot(*this);
    }
    
    Vector3d minimum(const Vector3d &v) const {
        return Vector3d(std::min(x, v.x), std::min(y, v.y), std::min(z, v.z));
    }

    Vector3d maximum(const Vector3d &v) const {
        return Vector3d(std::max(x, v.x), std::max(y, v.y), std::max(z, v.z));
    }
    
    std::string toString() const {
        std::stringstream ss;
        ss << "(" << x << ", " << y << ", " << z << ")";
        return ss.str();
    }
    
    double x, y, z;
};

Vector3d operator+(const Vector3d &v1, const Vector3d &v2) {
    Vector3d ret = v1;
    ret += v2;
    return ret;
}

Vector3d operator-(const Vector3d &v1, const Vector3d &v2) {
    Vector3d ret = v1;
    ret -= v2;
    return ret;
}

Vector3d operator*(const Vector3d &v, double s) {
    Vector3d ret = v;
    ret *= s;
    return ret;
}

Vector3d operator*(double s, const Vector3d &v) {
    Vector3d ret = v;
    ret *= s;
    return ret;
}

std::ostream & operator<<(std::ostream &os, const Vector3d &v) {
    os << v.toString();
    return os;
}
    
}  // namespace MeshVolume

#endif  // _MESHVOLUME_POINT3D_H_
