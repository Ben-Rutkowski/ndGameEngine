#include "edit_space_objects/edit_mesh.hpp"

// ================ Initialization ================
EditMesh::EditMesh() {
    initializeWithDefaultCube();
}

void EditMesh::initializeWithDefaultCube() {
    // --- Initialize Vertex Stroage ---
    // point_vertices.reserve(8);
    point_vertices = std::vector<V_EditMesh_pt>(8);
    
    // --- Initialize Index Storage ---
    edge_indices.reserve(24);
    tri_indices.reserve(36);

    // --- Initialize Mesh Storage ---
    half_edges_dcel.reserve(24);
    faces_dcel.reserve(8);

    // --- Create Point Vertices ---
    point_vertices[0].position = { -0.5f, -0.5f,  0.5f,  1.0f  };
    point_vertices[1].position = {  0.5f, -0.5f,  0.5f,  1.0f  };
    point_vertices[2].position = {  0.5f, -0.5f, -0.5f,  1.0f  };
    point_vertices[3].position = { -0.5f, -0.5f, -0.5f,  1.0f  };

    point_vertices[4].position = { -0.5f,  0.5f,  0.5f,  1.0f  };
    point_vertices[5].position = {  0.5f,  0.5f,  0.5f,  1.0f  };
    point_vertices[6].position = {  0.5f,  0.5f, -0.5f,  1.0f  };
    point_vertices[7].position = { -0.5f,  0.5f, -0.5f,  1.0f  };
    
    // --- Create Faces ---
    unsigned int bottom = addNewFace({0, 1, 2, 3});
    unsigned int top    = addNewFace({4, 5, 6, 7});
    unsigned int front  = addNewFace({0, 1, 5, 4});
    unsigned int right  = addNewFace({1, 2, 6, 5});
    unsigned int back   = addNewFace({2, 3, 7, 6});
    unsigned int left   = addNewFace({3, 0, 4, 7});

    // --- Stich Faces ---
    stitchFaces(front, right, 1, 3);
    stitchFaces(right, back, 1, 3);
    stitchFaces(back, left, 1, 3);
    stitchFaces(left, front, 1, 3);

    stitchFaces(bottom, front, 0, 0);
    stitchFaces(bottom, right, 1, 0);
    stitchFaces(bottom, back, 2, 0);
    stitchFaces(bottom, left, 3, 0);

    stitchFaces(top, front, 0, 2);
    stitchFaces(top, right, 1, 2);
    stitchFaces(top, back, 2, 2);
    stitchFaces(top, left, 3, 2);

    debug();
}


// ================ Creating Objects ================
unsigned int EditMesh::addNewFace(const std::array<unsigned int,4> points) {
    // --- Create Half-Edges ---
    unsigned int first_edge = half_edges_dcel.size();

    half_edges_dcel.push_back({  points[0], first_edge+1, first_edge+3, 0  });
    half_edges_dcel.push_back({  points[1], first_edge+2, first_edge,   0  });
    half_edges_dcel.push_back({  points[2], first_edge+3, first_edge+1, 0  });
    half_edges_dcel.push_back({  points[3], first_edge,   first_edge+2, 0  });

    // --- Create Face ---
    unsigned int face = faces_dcel.size();
    
    faces_dcel.push_back({  first_edge, 4  });

    // --- Create Tri Indices ---
    tri_indices.push_back(points[0]);
    tri_indices.push_back(points[1]);
    tri_indices.push_back(points[2]);
    tri_indices.push_back(points[2]);
    tri_indices.push_back(points[3]);
    tri_indices.push_back(points[0]);
    
    return face;
}


// ================ Editing Objects ================
void EditMesh::stitchFaces(unsigned int face_0,
                 unsigned int face_1,
                 unsigned int N_e0,
                 unsigned int N_e1)
{
    unsigned int edge_0 = faces_dcel[face_0].inner_hedge;
    unsigned int edge_1 = faces_dcel[face_1].inner_hedge;

    for (int i=0; i<N_e0; i++) {
        edge_0 = half_edges_dcel[edge_0].next_hedge;
    }

    for (int i=0; i<N_e1; i++) {
        edge_1 = half_edges_dcel[edge_1].next_hedge;
    }
    
    half_edges_dcel[edge_0].twin_hedge = edge_1;
    half_edges_dcel[edge_1].twin_hedge = edge_0;
}


// ================ Debugging ================
void EditMesh::debug() {
    int N_p = point_vertices.size(); 

    std::cout << "Points : " << N_p << std::endl;
    for (int i=0; i<N_p; i++) {
        std::cout << point_vertices[i].position[0] << " ";
        std::cout << point_vertices[i].position[1] << " ";
        std::cout << point_vertices[i].position[2] << " ";
        std::cout << point_vertices[i].position[3] << " " << std::endl;;
    }
}
