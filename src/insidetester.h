#ifndef _MESHVOLUME_INSIDETESTER_H_
#define _MESHVOLUME_INSIDETESTER_H_

#include <ctime>
#include <vector>
#include <map>
#include <memory>

#include "common.h"
#include "random.h"
#include "mesh.h"
#include "bounds2d.h"
#include "bounds3d.h"

namespace MeshVolume {

using IndexedAABB = std::pair<int, Bounds2d>;

struct BVHNode {
    BVHNode()
        : index(-1)
        , bounds() {
    }

    BVHNode(int i, const Bounds2d &b)
        : index(i)
        , bounds(b) {
    }
    
    bool isLeaf() const {
        return index >= 0;
    }

    int index = -1;
    Bounds2d bounds;
    BVHNode *left = nullptr;
    BVHNode *right = nullptr;
};

class InsideTester {
public:
    InsideTester() {
    }

    explicit InsideTester(const std::shared_ptr<Mesh> &mesh_) {
        this->mesh = mesh_;
        initialize(*mesh_);
    }
    
    void initialize(const Mesh &mesh) {
        const int nFaces = (int)mesh.faces.size();
        
        Bounds2d bounds;
        std::vector<IndexedAABB> AABBs;
        for (int i = 0; i < nFaces; i++) {
            const Triangle &t = mesh.faces[i];
            Bounds2d b;
            b.extend(Vector2d(t.v0.x, t.v0.y));
            b.extend(Vector2d(t.v1.x, t.v1.y));
            b.extend(Vector2d(t.v2.x, t.v2.y));
            AABBs.emplace_back(i, b);
            
            bounds.extend(b);
        }
        jitter = EPS * bounds.maxExtent();
        
        int nNodes = 1;
        while (nNodes < nFaces) nNodes <<= 1;
        nNodes <<= 1;
        nodes = std::unique_ptr<BVHNode[]>(new BVHNode[nNodes]);
        
        construct(0, bounds, 0, AABBs);
    }
    
    BVHNode * construct(int index, const Bounds2d &bounds, int depth, std::vector<IndexedAABB> &AABBs) {
        if (AABBs.empty()) {
            return nullptr;
        }

        if (AABBs.size() == 1) {
            nodes[index] = BVHNode(AABBs[0].first, AABBs[0].second);
            return &nodes[index];
        }
        
        const int axis = depth % 2;
        std::sort(AABBs.begin(), AABBs.end(),
                  [=](const IndexedAABB &b1, const IndexedAABB &b2) {
                      return b1.second.center()[axis] < b2.second.center()[axis];
                  });

        Bounds2d leftBB, rightBB;
        std::vector<IndexedAABB> leftAABBs, rightAABBs;
        for (int i = 0; i < AABBs.size() / 2; i++) {
            leftBB.extend(AABBs[i].second);
            leftAABBs.push_back(AABBs[i]);
        }
        
        for (int i = AABBs.size() / 2; i < AABBs.size(); i++) {
            rightBB.extend(AABBs[i].second);
            rightAABBs.push_back(AABBs[i]);
        }
        
        nodes[index].bounds = bounds;
        nodes[index].left   = construct(index * 2 + 1, leftBB, depth + 1, leftAABBs);
        nodes[index].right  = construct(index * 2 + 2, rightBB, depth + 1, rightAABBs);
        
        return &nodes[index];
    }
    
    bool inside(const Vector3d &v) const {
        indices.clear();
        indices.reserve(1024);
        collectAABBs(&nodes[0], Vector2d(v.x, v.y));
        
        Random rand((unsigned long)time(0));
        for (int trial = 0; trial < 4; trial++) {
            double rx = rand.nextReal() * 2.0 - 1.0;
            double ry = rand.nextReal() * 2.0 - 1.0;
            Vector2d u(v.x + rx * jitter, v.y + ry * jitter);

            int count = 0;
            for (int index : indices) {
                Triangle &t = mesh->faces[index];
                Vector2d v0(t.v0.x, t.v0.y);
                Vector2d v1(t.v1.x, t.v1.y);
                Vector2d v2(t.v2.x, t.v2.y);
                double d0 = (v0 - u).det(v1 - u);
                double d1 = (v1 - u).det(v2 - u);
                double d2 = (v2 - u).det(v0 - u);
                if (d0 * d1 >= 0.0 && d0 * d2 >= 0.0) {
                    Vector2d e1 = v1 - v0;
                    Vector2d e2 = v2 - v0;
                    Vector2d e = u - v0;
                    double det = e1.x * e2.y - e1.y * e2.x;
                    if (det == 0.0) continue;

                    double t1 = ( e2.y * e.x - e2.x * e.y) / det;
                    double t2 = (-e1.y * e.x + e1.x * e.y) / det;
                    double z = (1.0 - t1 - t2) * t.v0.z + t1 * t.v1.z + t2 * t.v2.z;
                    Vector2d vv = t1 * e1 + t2 * e2;
                    if (v.z < z) {
                        count += 1;
                    }
                }
            }
            if (count % 2 != 0) return true;
        }
        return false;
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
        
        if (isInside ) return (int)Relation::Inside;
        if (isOutside) return (int)Relation::Outside;
        return (int)Relation::Intersect;
    }
    
    void collectAABBs(const BVHNode *node, const Vector2d &v) const {
        if (node->bounds.inside(v)) {
            if (node->isLeaf()) {
                indices.push_back(node->index);
                return;
            } else {
                if (node->left ) collectAABBs(node->left , v);
                if (node->right) collectAABBs(node->right, v);
            }
        }
    }
    
private:
    std::shared_ptr<Mesh> mesh;
    std::unique_ptr<BVHNode[]> nodes;
    mutable std::vector<int> indices;
    double jitter = 1.0e-8;
};

}  // namespace MeshVolume

#endif  // _MESHVOLUME_INSIDETESTER_H_
