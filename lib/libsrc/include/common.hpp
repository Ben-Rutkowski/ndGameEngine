#ifndef COMMON_HPP
#define COMMON_HPP

#include "glad_glfw.hpp"
#include "enum_map.hpp"

// === Event Enums ===
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

// === Shader Enums ===
enum class ShaderType { null = 0,
    VERTEX, FRAGMENT, GEOMETRY,
    LEN
};

#define EVENT_DATA_LEN  (int)Data::LEN
#define SHADER_TYPE_LEN (int)ShaderType::LEN

// === Enum Maps ===
typedef EnumMap<ShaderType, GLenum, SHADER_TYPE_LEN> ShaderEnumMap;

namespace enumUtil {

ShaderEnumMap getShaderEnumMap();
}

// === CONSTANTS ===
#define ROOT "/Users/benjaminrutkowski/Projects/ndGameEngine/"

#endif