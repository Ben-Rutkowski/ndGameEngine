#include "window.hpp"
#include "math/matrix.hpp"
#include "shader_types.h"
#define DEBUG_BREAK 5
#define DEBUG_KILL  5
static int DEBUG_count;


void ndWindowModule::onBeginStartUp(ndEvent* event) {
    DEBUG_count = 0;
}

void ndWindowModule::onEndStartUp(ndEvent* event) {
    nd_window.showWindow();
    pollEventsCocoa();
}

void ndWindowModule::onDebug(ndEvent* event) {
    DEBUG_count += 1;
    if (DEBUG_count == DEBUG_BREAK) 
    {
        
    } 
    
    if (DEBUG_count == DEBUG_KILL) {
        killCocoa();
    }
}