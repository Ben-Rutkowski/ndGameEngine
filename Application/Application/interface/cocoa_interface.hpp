#ifndef COCOA_INTERFACE_HPP
#define COCOA_INTERFACE_HPP

// ======== Static Variables ========
static void* CURRENT_APPLICATION_DELEGATE_COCOA;


// ======== Application ========
int initCocoa();
void pollEventsCocoa();


// ======== Window ========
class ndWindow {
private:
    void* WindowCOCOA;
    void* WindowDelegateCOCOA;
    void* MTKViewCOCOA;
    void* MTKRendererCOCOA;
    
public:
    ndWindow(int width, int height, const char* title);
    
// --- Interface ---
    bool shouldClose();
    
    void setClearColor(double r, double g, double b, double a);
    void showWindow();
    
    void drawView();

// --- Debugging ---
public:
    void debug();
};

#endif
