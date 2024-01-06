#ifndef COCOA_INTERFACE_HPP
#define COCOA_INTERFACE_HPP


// ======== Draw Routine Enums ========
typedef enum ndDrawRoutineKind {
    ndDrawRoutineKindNull  = 0,
    ndDrawRoutineKindDebug = 1,
    ndDrawRoutineKindLine  = 2
} ndDrawRoutineKind;


#ifndef __IN_COCOA__

// ======== Static Variables ========
static void* CURRENT_APPLICATION_DELEGATE_COCOA;


// ======== Application ========
int  initCocoa();
void pollEventsCocoa();


// ======== Routine ========
class ndRoutine {
private:
    void* routine_ptr_COCOA;
    unsigned long routine_index;
    
public:
    ndRoutine(void* renderer_ptr, unsigned long index);

    void bindBuffer(unsigned int buffer_index);
    void createBuffer(unsigned int vertex_count);
    
    void* writeBufferOpen();
    void  writeBufferClose();
    
    unsigned long index();
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
    
// ==== Draw ====
    void drawView();
    
// ==== Renderer ====
    ndRoutine createDrawRoutine(ndDrawRoutineKind draw_routine_kind);
    void configureRoutine();
    void bindRoutine(ndRoutine routine);
    void armRoutine();
};

#endif /* ifndef __IN_COCOA__ */

#endif
