#ifndef COMMON_HPP
#define COMMON_HPP

#include "glad_glfw.hpp"
#include "enum_map.hpp"
#include "math/matrix.hpp"

#include <string>

// === Event Enums ===
enum class EventType { null = 0, 
    ACTION, VEC2I, VEC2F, VEC4F, VEC4I,
    LEN
};

enum class Module { null = 0, TEST, 
    APPLICATION, WINDOW, EDIT_SPACE,
    LEN
};

enum class Data { null = 0, TEST, DEBUG,
    /* Operations */ CLOSE_APP, RESIZE_FRAME, RESIZE_WINDOW, 
    /* Runtime */    BEGIN_LOOP, START_FRAME, DRAW_WINDOW_FRAME, DRAW, END_FRAME,          
    /* Key Events */ COLLECT_MENU_KEYS, ESCAPE_KEY, SCROLL,
                     W_KEY, A_KEY, S_KEY, D_KEY, G_KEY_PRESS, G_KEY_RELEASE,
                     RIGHT_MOUSE_CLICK, RIGHT_MOUSE_HOLD, RIGHT_MOUSE_RELEASE,
                     LEFT_MOUSE_CLICK, LEFT_MOUSE_HOLD, LEFT_MOUSE_RELEASE,
                     SHIFT_PRESS, SHIFT_RELEASE,
    /* Timers */     CLICK_DEBUG_TIMER, DELTA_DEBUG_TIMER,
    LEN
};

enum class Request { null = 0,
    /* Window */ SHOULD_CLOSE, MOUSE_POSITION, FRAME_DELTA,
    LEN
};

// === Shader Enums ===
enum class ShaderType { null = 0,
    VERTEX, FRAGMENT, GEOMETRY,
    LEN
};

// === Clock Enums ===
enum class Watch { null = 0,
    FRAMERATE, FRAME_DELTA, DEBUG, 
    LEN
};

#define WATCH_LEN       (int)Watch::LEN
#define MODULE_LEN      (int)Module::LEN
#define EVENT_DATA_LEN  (int)Data::LEN
#define SHADER_TYPE_LEN (int)ShaderType::LEN

// === Enum Maps ===
typedef EnumMap<ShaderType, GLenum, SHADER_TYPE_LEN> ShaderEnumMap;
typedef EnumMap<Data, std::string, EVENT_DATA_LEN>   EventNameMap;
typedef EnumMap<Module, std::string, MODULE_LEN>     ModuleNameMap; 

class EnumMapIndex {
private:
    static ShaderEnumMap shader_enum_map;
    static EventNameMap  event_name_map;
    static ModuleNameMap module_name_map;

public:
    static GLenum      glShader(ShaderType type);
    static std::string dataName(Data data);
    static std::string moduleName(Module moduel);
};

// === SRUCTS ===

struct emMaterial {
    vec4 ambient;
    vec4 diffuse;
    vec4 select_color;
};

struct CameraPack {
    vec4 front;
    vec4 right;
    vec4 pos;

    mat4 view;
    mat4 proj;
};

// === CONSTANTS ===
#define ROOT "/Users/benjaminrutkowski/Projects/ndGameEngine/"

#endif