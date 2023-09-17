#ifndef EDIT_POINT_HPP
#define EDIT_POINT_HPP

#include "em_sub_objects/edit_cache.hpp"

class EditPoint {
// ---- Attributes ----
private:
    vec4  position;
    float select;

public:
    EditPoint(vec4 pos_in);

// --- Gets and Sets ---
public:
    vec4 getPos();
    bool getSelect();
    
    void setPos(vec4 pos_in);
    void setSelect(bool value);
};

/* CLASS: PointCache

A PointCache has as data vec4 for positions and has edge and face indices.
pair cache1: edge
pair cache2: face
pair cache3: vertex

*/

class PointCache : public EditCache<EditPoint> {
public:
    Id addPoint(vec4 position) {
        pushPairCache1(4);
        pushPairCache2(4);
        pushPairCache3(1);
        EditPoint point(position);
        return addData(point);
    }

    void pairEdge(Id point_id, Id edge_id) 
        { pairIndexCache1(point_id, edge_id); }
    void pairFace(Id point_id, Id face_id) 
        { pairIndexCache2(point_id, face_id); }
    void pairVertex(Id point_id, Id vertex_id) 
        { pairIndexCache3(point_id, vertex_id); }

    int pairedEdgeLen(Id point_id)   { return pairedLen1(point_id); }
    int pairedFaceLen(Id point_id)   { return pairedLen2(point_id); }
    int pairedVertexLen(Id point_id) { return pairedLen3(point_id); }

    Id getPairedEdge(Id point_id, int i)
        { return getPairedCache1(point_id, i); }
    Id getPairedFace(Id point_id, int i)
        { return getPairedCache2(point_id, i); }
    Id getPairedVertex(Id point_id, int i)
        { return getPairedCache3(point_id, i); }

    void depairPoint(Id point_id) {
        emptyIndexCache1(point_id);
        emptyIndexCache2(point_id);
        emptyIndexCache3(point_id);
    }

    void removeEdge(Id point_id, Id edge_id) {
        int N = pairedEdgeLen(point_id);
        for (int i=0; i<N; i++) {
            if (pair_cache_1[point_id][i] == edge_id) {
                pair_cache_1[point_id].erase(pair_cache_1[point_id].begin()+i);
            }
        }
    }

    void removeFace(Id point_id, Id face_id) {
        int N = pairedFaceLen(point_id);
        for (int i=0; i<N; i++) {
            if (pair_cache_2[point_id][i] == face_id) {
                pair_cache_2[point_id].erase(pair_cache_2[point_id].begin()+i);
            }
        }
    }

    // void replaceVertex(Id point_id, Id old_vert_id, Id new_vert_id) {
    //     int N = pairedVertexLen(point_id);
    //     for (int i=0; i<N; i++) {
    //         if (pair_cache_3[point_id][i] == old_vert_id) {
    //             pair_cache_3[point_id][i] = new_vert_id;
    //             break;
    //         }
    //     }
    // }

    void removeVertex(Id point_id, Id vert_id) {
        int N = pairedVertexLen(point_id);
        for (int i=0; i<N; i++) {
            if (pair_cache_3[point_id][i] == vert_id) {
                pair_cache_3[point_id].erase(pair_cache_3[point_id].begin()+i);
            }
        }
    }
};

#endif