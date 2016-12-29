#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "sphere.h"
#include "mesh.h"
#include "insidetester.h"

using namespace MeshVolume;

const std::string filename = "../../../data/bunny.obj";

void progressBar(int x, int total, int width = 50) {
    double ratio = (double)x / total;
    int tick = (int)(width * ratio);
    std::string bar(width, ' ');
    std::fill(bar.begin(), bar.begin() + tick, '+');
    printf("[ %6.2f %% ] [ %s ]", 100.0 * ratio, bar.c_str());
    printf("%c", x >= total ? '\n' : '\r');
}

double volume(const InsideTester &mesh, const Sphere &sph, const Bounds3d &bounds, int depth = 0, double tol=1.0e-6) {
    // Tolerance
    if (bounds.volume() < tol) {
        return 0.0;
    }

    // Inside/outside test
    int relate = sph.relation(bounds);
    if (relate == Relation::Inside) {
        relate = mesh.relation(bounds);
        if (relate == Relation::Inside) {
            return bounds.volume();
        } else if (relate == Relation::Outside) {
            return 0.0;
        }
    } else if (relate == Relation::Outside) {
        return 0.0;
    }

    // Split BBox
    const int axis = depth % 3;
    Bounds3d leftBox, rightBox;
    Vector3d center = bounds.center();
    if (axis == 0) {
        leftBox = Bounds3d(bounds.posMin, Vector3d(center.x, bounds.posMax.y, bounds.posMax.z));
        rightBox = Bounds3d(Vector3d(center.x, bounds.posMin.y, bounds.posMin.z), bounds.posMax);
    } else if (axis == 1) {
        leftBox = Bounds3d(bounds.posMin, Vector3d(bounds.posMax.x, center.y, bounds.posMax.z));
        rightBox = Bounds3d(Vector3d(bounds.posMin.x, center.y, bounds.posMin.z), bounds.posMax);
    } else if (axis == 2) {
        leftBox = Bounds3d(bounds.posMin, Vector3d(bounds.posMax.x, bounds.posMax.y, center.z));
        rightBox = Bounds3d(Vector3d(bounds.posMin.x, bounds.posMin.y, center.z), bounds.posMax);
    }
    
    // Compute for subvolume
    double leftV = volume(mesh, sph, leftBox, depth + 1, tol);
    double rightV = volume(mesh, sph, rightBox, depth + 1, tol);
    return leftV + rightV;
}

int main(int argc, char **argv) {
    // Load mesh
    auto mesh = std::shared_ptr<Mesh>(new Mesh(filename));
    InsideTester tester(mesh);
    
    // BBox for bunny
    Bounds3d bbox;
    for (int i = 0; i < mesh->faces.size(); i++) {
        bbox.extend(mesh->faces[i].v0);
        bbox.extend(mesh->faces[i].v1);
        bbox.extend(mesh->faces[i].v2);
    }
    Vector3d diff = bbox.posMax - bbox.posMin;
    double radius = 0.1 * std::max(std::abs(diff.x), std::max(std::abs(diff.y), std::abs(diff.z)));
    
    // Compute volumes
    std::vector<double> volumes(mesh->vertices.size());
    for (int i = 0; i < mesh->vertices.size(); i++) {
        Sphere sph(mesh->vertices[i], radius);
        volumes[i] = volume(mesh, sph, bbox);
        if (i % 100 == 0) {
            progressBar(i + 1, mesh->vertices.size());
            std::fflush(stdout);
        }
    }
}
