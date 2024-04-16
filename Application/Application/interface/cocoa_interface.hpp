#ifndef COCOA_INTERFACE_HPP
#define COCOA_INTERFACE_HPP


// ======== Enums ========
typedef enum CocoaCallback_E {
    CocoaCB_Resize = 0
} CocoaCallback_E;


// ======== Static Variables ========
static void* CURRENT_APPLICATION_DELEGATE_COCOA;

typedef void(*CocoaEventCallBack)(void*, unsigned int);
typedef void(*CocoaEventCallBackVI2)(void*, unsigned int, int, int);
extern  void*                 EVENT_MANAGER_PTR;
extern  CocoaEventCallBack    COCOA_CALLBACK;
extern  CocoaEventCallBackVI2 COCOA_CALLBACK_VI2;


#ifdef __IN_COCOA__
#define __IN_COCOA__
#import <Foundation/Foundation.h>

@interface ndCallBackCenter : NSObject
+ (void) callback:(CocoaCallback_E)type;
+ (void) callback:(CocoaCallback_E)type
       withIntOne:(int)int_one
       withIntTwo:(int)int_two;
+ (void) debug;
@end
#endif


#ifndef __IN_COCOA__
#include "draw_routine_indices.h"

// ======== Application ========
int  initCocoa();
void pollEventsCocoa();
void killCocoa();


// ======== Callbacks ========
void setEventManagerCocoa(void* event_manager_ptr);
void setEventCallbackCocoa(CocoaEventCallBack callback);
void setEventCallbackCocoaVI2(CocoaEventCallBackVI2 callback);


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

    void  writeBufferModify(int loc, int len);
    
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
