#ifndef COCOA_INTERFACE_HPP
#define COCOA_INTERFACE_HPP


// ======== Draw Routine Enums ========
typedef enum ndDrawRoutineKind {
    ndDrawRoutineKindNull  = 0,
    ndDrawRoutineKindDebug = 1
} ndDrawRoutineKind;


#ifndef __IN_COCOA__

// ======== Static Variables ========
static void* CURRENT_APPLICATION_DELEGATE_COCOA;


// ======== Application ========
int  initCocoa();
void pollEventsCocoa();


// ================ ndBuffer ================
class ndBuffer {
private:
    void* buffer_ptr_COCOA;
    
public:
    ndBuffer(void* buffer_ptr);
    
    void* editTap();
    void  editUntap();
    
    unsigned long size();
    
    void debug();
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
    
// ==== Routine Interface ====
    unsigned int createDrawRoutine(ndDrawRoutineKind draw_routine_kind);
    void bindRoutine(unsigned int draw_routine_index);
    void configureRoutine();
    void armRoutine();
    
    void     bindBuffer(unsigned int buffer_index);
    void     createBuffer(unsigned int vertex_count);
    ndBuffer getBuffer();
    
    void  createBufferOLD(unsigned int vertex_count);
    void* getBufferOLD();
};

#endif /* ifndef __IN_COCOA__ */

#endif
