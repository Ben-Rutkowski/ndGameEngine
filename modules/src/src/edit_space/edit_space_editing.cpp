#include "edit_space.hpp"

Id EditSpace::createMesh() {
    meshes.push_back(EditMesh());
    return meshes.size() - 1;
}

Id EditSpace::createDefaultCube() {
    Id cube_id = createMesh();
    EditMesh& cube = ref(cube_id);

    Id4 bp = {
        cube.createPoint(vec4({-0.5f, -0.5f,  0.5f, 1.0f})),
        cube.createPoint(vec4({ 0.5f, -0.5f,  0.5f, 1.0f})),
        cube.createPoint(vec4({ 0.5f, -0.5f, -0.5f, 1.0f})),
        cube.createPoint(vec4({-0.5f, -0.5f, -0.5f, 1.0f}))
    };

    Id4 tp = {
        cube.createPoint(vec4({-0.5f,  0.5f,  0.5f, 1.0f})),
        cube.createPoint(vec4({ 0.5f,  0.5f,  0.5f, 1.0f})),
        cube.createPoint(vec4({ 0.5f,  0.5f, -0.5f, 1.0f})),
        cube.createPoint(vec4({-0.5f,  0.5f, -0.5f, 1.0f}))
    };

    // Edges
    Id4 be; Id4 te; Id4 se;
    for (int i=0; i<4; i++) {
        be[i] = cube.createEdge({bp[i%4], bp[(i+1)%4]});
        te[i] = cube.createEdge({tp[i%4], tp[(i+1)%4]});
        se[i] = cube.createEdge({bp[i%4], tp[i%4]});
    }

    // Quads
    cube.createQuad(tp, te);
    cube.createQuad(
        {bp[3], bp[2], bp[1], bp[0]},
        {be[3], be[2], be[1], be[0]}
    );
    for (int i=0; i<4; i++) {
        cube.createQuad(
            {bp[i%4], bp[(i+1)%4], tp[(i+1)%4], tp[i%4]},
            {be[i%4], se[(i+1)%4], te[i%4], se[i%4]}
        );
    }
    // cube.translate(vec4({0.0f, 0.5f, 0.0f, 1.0f}));
    return cube_id;
}

void EditSpace::selectPointsBox() {
    vec4 v1 = camera.clipToWorld(select_box.getRoot());
    vec4 v2 = camera.clipToWorld(select_box.getBasisX());
    vec4 v3 = camera.clipToWorld(select_box.getBasisY());

    meshes[0].clearSelectedPoints();
    meshes[0].selectPointsBox(v1, v2, v3, camera.getPos());
}

void EditSpace::selectFacesClick(vec2 click) {
    vec4 point = camera.clipToWorld(click);
    meshes[0].selectFacesClick(point, camera.getPos());
}
