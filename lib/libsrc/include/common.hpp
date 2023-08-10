#ifndef COMMON_HPP
#define COMMON_HPP

// === Enums ===

enum class EventType { null = 0, 
    
    ACTION, VEC2I, 
    
    LEN
};

enum class Module { null = 0, TEST, 
    
    APPLICATION, WINDOW, 
    
    LEN
};

enum class Data { null = 0, TEST,
    
    /* Operations */ CLOSE_APP, RESIZE,  
    /* Runtime */    START_FRAME, END_FRAME,          
    /* Key Events */ COLLECT_MENU_KEYS, ESCAPE_KEY, 
    
    LEN
};

#define EVENT_DATA_LEN (int)Data::LEN

#endif