#include "common.hpp"

ShaderEnumMap getShaderEnumMap() {
    ShaderEnumMap map;
    map.addEntry(ShaderType::VERTEX,   GL_VERTEX_SHADER);
    map.addEntry(ShaderType::FRAGMENT, GL_FRAGMENT_SHADER);
    map.addEntry(ShaderType::GEOMETRY, GL_GEOMETRY_SHADER);

    return map;
}

EventNameMap getEventNameMap() {
    EventNameMap map;
    map.addEntry(Data::TEST,              std::string("TEST"));
    map.addEntry(Data::CLOSE_APP,         std::string("CLOSE_APP"));
    map.addEntry(Data::RESIZE_FRAME,      std::string("RESIZE_FRAME"));
    map.addEntry(Data::RESIZE_WINDOW,     std::string("RESIZE_WINDOW"));
    map.addEntry(Data::START_FRAME,       std::string("START_FRAME"));
    map.addEntry(Data::DRAW_FRAME,        std::string("DRAW_FRAME"));
    map.addEntry(Data::END_FRAME,         std::string("END_FRAME"));
    map.addEntry(Data::COLLECT_MENU_KEYS, std::string("COLLECT_MENU_KEYS"));
    map.addEntry(Data::ESCAPE_KEY,        std::string("ESCAPE_KEY"));
    map.addEntry(Data::BEGIN_LOOP,        std::string("BEGIN_LOOP"));
    map.addEntry(Data::RIGHT_MOUSE_CLICK, std::string("RIGHT_MOUSE_CLICK"));
    map.addEntry(Data::RIGHT_MOUSE_HOLD,  std::string("RIGHT_MOUSE_HOLD"));
    map.addEntry(Data::LEFT_MOUSE_CLICK,  std::string("LEFT_MOUSE_CLICK"));
    map.addEntry(Data::LEFT_MOUSE_HOLD,   std::string("LEFT_MOUSE_HOLD"));
    map.addEntry(Data::RIGHT_MOUSE_RELEASE, std::string("RIGHT_MOUSE_RELEASE"));
    map.addEntry(Data::LEFT_MOUSE_RELEASE,  std::string("LEFT_MOUSE_RELEASE"));
    map.addEntry(Data::SCROLL,              std::string("SCROLL"));
    map.addEntry(Data::W_KEY,               std::string("W_KEY"));
    map.addEntry(Data::A_KEY,               std::string("A_KEY"));
    map.addEntry(Data::S_KEY,               std::string("S_KEY"));
    map.addEntry(Data::D_KEY,               std::string("D_KEY"));

    return map;
}

ModuleNameMap getModuleNameMap() {
    ModuleNameMap map;
    map.addEntry(Module::APPLICATION, std::string("APPLICATION"));
    map.addEntry(Module::WINDOW,      std::string("WINDOW"));
    map.addEntry(Module::EDIT_SPACE,  std::string("EDIT_SPACE"));

    return map;
}

ShaderEnumMap EnumMapIndex::shader_enum_map = getShaderEnumMap();
EventNameMap  EnumMapIndex::event_name_map  = getEventNameMap();
ModuleNameMap EnumMapIndex::module_name_map = getModuleNameMap();

GLenum EnumMapIndex::glShader(ShaderType type) {
    return shader_enum_map.get(type);
}

std::string EnumMapIndex::dataName(Data data) {
    return event_name_map.get(data);
}

std::string EnumMapIndex::moduleName(Module module) {
    return module_name_map.get(module);
}