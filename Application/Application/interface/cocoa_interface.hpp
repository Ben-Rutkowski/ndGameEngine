#ifndef COCOA_INTERFACE_HPP
#define COCOA_INTERFACE_HPP

// ======== Static Variables ========
static void* CURRENT_APPLICATION_DELEGATE_COCOA;


// ======== Enums ========
typedef enum ndDrawRoutineKind {
    ndDrawRoutineKindDebug = 0
} ndDrawRoutineKind;


// ======== Application ========
int  initCocoa();
void pollEventsCocoa();


// ======== Window ========
class ndWindow {
private:
    void* WindowCOCOA;
    void* WindowDelegateCOCOA;
    void* ndViewCOCOA;
    
public:
    ndWindow(int width, int height, const char* title);
    
// --- Interface ---
    bool shouldClose();
    void showWindow();
    void drawView();
    
    unsigned int initDrawRoutine(ndDrawRoutineKind draw_routine_kind);

// --- Debugging ---
public:
    void debug();
};

#endif
