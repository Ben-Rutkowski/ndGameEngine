#include "cocoa_interface.hpp"
#include "window.hpp"
#define __INTERNAL__
#define DEBUG_BREAK 1
#define DEBUG_KILL  1
static int DEBUG_count;

void ndWindowModule::onBeginStartUp(ndEvent* event) {

}

void ndWindowModule::onEndStartUp(ndEvent* event) {
    nd_window.showWindow();
    pollEventsCocoa();
}

void ndWindowModule::onDraw(ndEvent* event) {

}

void ndWindowModule::onDebug(ndEvent* event) {
    DEBUG_count += 1;
    if (DEBUG_count == DEBUG_BREAK) 
    {

    } 
    
    // if (DEBUG_count == DEBUG_KILL) {
    //     killCocoa();
    // }
}
