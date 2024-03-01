#ifndef COCOA_INTERFACE_HPP
#define COCOA_INTERFACE_HPP

#include "draw_routine_indices.h"

#ifndef __IN_COCOA__

// ======== Static Variables ========
static void* CURRENT_APPLICATION_DELEGATE_COCOA;


// ======== Application ========
int  initCocoa();
void pollEventsCocoa();
void killCocoa();


// ======== Routine ========
typedef struct ScreenSize {
    double width;
    double height;
} ScreenSize;

class ndRoutine {
private:
    void* routine_ptr_COCOA;
    unsigned long routine_index;
    
public:
    ndRoutine(void* renderer_ptr, unsigned long index);

    void bindBuffer(unsigned int buffer_index);
    // void createPublicBuffer(unsigned int vertex_size, unsigned int vertex_count);
    // void createAuxBuffer(unsigned int vertex_size, unsigned int vertex_count);

    void createBuffer(Buffer_T buffer_type, unsigned int vertex_size, unsigned int vertex_count);
    
    void* writeBufferOpen();
    void  writeBufferClose();
    
    unsigned long index();
    
    void debug(unsigned int vertex_count);
    
//    --- Depricated ---
    void createBuffer(unsigned int vertex_count);
};


// ======== Window ========
class ndWindow {
private:
    void* window_COCOA;
    void* window_delegate_COCOA;
    void* nd_view_COCOA;
    void* render_switcher_COCOA;
    
public:
    ndWindow(int width, int height, const char* title);
    
// ==== Window Operations ====
    bool shouldClose();
    void showWindow();

    ScreenSize getScreenSize();
    
// ==== Draw ====
    void drawView();
    
// ==== Renderer ====
    ndRoutine createDrawRoutine(DrawRoutineKind draw_routine_kind);
    void armRoutine(ndRoutine routine);
};

#endif /* ifndef __IN_COCOA__ */

#endif
