#ifndef EDIT_SPACE_HPP
#define EDIT_SPACE_HPP

#include "nd_module.hpp"
#include "edit_space_objects/edit_mesh.hpp"

typedef enum ndEditSpaceState { ndEditSpaceStateNull = 0,
    ndEditSpaceStateLen    
} ndEditSpaceState;


class ndEditSapceModule : public ndModule<ndEditSpaceStateLen> {
private:
    EditMesh test_mesh;

// --- Initialization ---
public:
    ndEditSapceModule();

// --- Events ---
public:
    void runEvent(ndEvent* event);

public:
    void onDebug(ndEvent* event);
    void onBeginStartUp(ndEvent* event);
    void onEndStartUp(ndEvent* event);
    void onStartFrame(ndEvent* event);
    void onEndFrame(ndEvent* event);
};

#endif
