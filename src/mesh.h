#ifndef _MESHVOLUME_MESH_H_
#define _MESHVOLUME_MESH_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "tiny_obj_loader.h"
#include "triangle.h"

namespace MeshVolume {

struct Mesh {
    Mesh()
        : faces() {
    }
    
    Mesh(const std::string &filename)
        : Mesh() {
        load(filename);
    }
    
    void load(const std::string &filename) {
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string errmsg;
        bool success = tinyobj::LoadObj(shapes, materials, errmsg, filename.c_str());
        if (!errmsg.empty()) {
            std::cerr << "[ ERROR ] " << errmsg << std::endl;
        }
        
        if (!success) {
            std::cerr << "[ ERROR ] Failed to load mesh: " << filename << std::endl;
            std::exit(1);
        }
        
        if (shapes.size() != 1) {
            std::cerr << "Mesh class does not support vertex group!!" << std::endl;
            std::exit(1);
        }

        auto &mesh = shapes[0].mesh;
        vertices.clear();
        for (int i = 0; i < mesh.positions.size(); i += 3) {
            vertices.emplace_back(mesh.positions[i + 0],
                                  mesh.positions[i + 1],
                                  mesh.positions[i + 2]);
        }
        
        faces.clear();
        for (int i = 0; i < mesh.indices.size(); i += 3) {
            faces.emplace_back(vertices[mesh.indices[i + 0]],
                               vertices[mesh.indices[i + 1]],
                               vertices[mesh.indices[i + 2]]);
        }
    }

    std::vector<Vector3d> vertices;
    std::vector<Triangle> faces;
};

}  // namespace MeshVolume

#endif  // _MESHVOLUME_MESH_H_
