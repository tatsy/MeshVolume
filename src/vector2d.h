#ifndef _MESHVOLUME_VECTOR2D_H_
#define _MESHVOLUME_VECTOR2D_H_

#include <cstdio>
#include <algorithm>

namespace MeshVolume {

struct Vector2d {
    explicit Vector2d(double x_ = 0.0, double y_ = 0.0)
        : x(x_)
        , y(y_) {
    }
    
    Vector2d(const Vector2d &v)
        : x(v.x)
        , y(v.y) {
    }
    
    Vector2d & operator=(const Vector2d &v) {
        this->x = v.x;
        this->y = v.y;
        return *this;
    }
    
    double operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        printf("Index out of bounds!!\n");
        std::exit(1);
        return 0.0;
    }
    
    Vector2d operator-() const {
        return Vector2d(-x, -y);
    }
    
    Vector2d & operator+=(const Vector2d &v) {
        this->x += v.x;
        this->y += v.y;
        return *this;
    }

    Vector2d & operator-=(const Vector2d &v) {
        this->x -= v.x;
        this->y -= v.y;
        return *this;
    }
    
    Vector2d & operator*=(double s) {
        this->x *= s;
        this->y *= s;
        return *this;
    }
    
    double dot(const Vector2d &v) const {
        return x * v.x + y * v.y;
    }
    
    double det(const Vector2d &v) const {
        return x * v.y - y * v.x;
    }
    
    Vector2d minimum(const Vector2d &v) const {
        return Vector2d(std::min(x, v.x), std::min(y, v.y));
    }

    Vector2d maximum(const Vector2d &v) const {
        return Vector2d(std::max(x, v.x), std::max(y, v.y));
    }
    
    double x, y;
};

Vector2d operator+(const Vector2d &v1, const Vector2d &v2) {
    Vector2d ret = v1;
    ret += v2;
    return ret;
}

Vector2d operator-(const Vector2d &v1, const Vector2d &v2) {
    Vector2d ret = v1;
    ret -= v2;
    return ret;
}

Vector2d operator*(const Vector2d &v, double s) {
    Vector2d ret = v;
    ret *= s;
    return ret;
}

Vector2d operator*(double s, const Vector2d &v) {
    Vector2d ret = v;
    ret *= s;
    return ret;
}
    
}  // namespace MeshVolume

#endif  // _MESHVOLUME_POINT2D_H_
