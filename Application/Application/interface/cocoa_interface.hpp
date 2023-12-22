#ifndef COCOA_INTERFACE_HPP
#define COCOA_INTERFACE_HPP

// --- Static Variables ---
static void* CURRENT_APPLICATION_DELEGATE_COCOA;

// --- Application ---
int initCocoa();
void pollEventsCocoa();

// --- Window ---
class ndWindow {
private:
    void* WindowCOCOA;
    void* WindowDelegateCOCOA;
    
public:
    ndWindow(int width, int height, const char* title);
    
    void debug();
};

#endif
