#include "common.hpp"

std::string moduleEnumToString(Module module) {
    std::string out;

    switch (module) {
    case Module::TEST: return "TEST";
    case Module::APPLICATION: return "APPLICATION";
    case Module::WINDOW: return "WINDOW";
    case Module::EDIT_SPACE: return "EDIT_SPACE";
    default: return "";
    }
}

std::string operationEnumToString(Operation operation) {
    std::string output;

    switch (operation) {
    case Operation::TEST: return "TEST";
    case Operation::DEBUG: return "DEBUG";
    case Operation::CLOSE_APP: return "CLOSE_APP";
    case Operation::RESIZE_FRAME: return "RESIZE_FRAME";
    case Operation::RESIZE_WINDOW: return "RESIZE_WINDOW";
    case Operation::START_FRAME: return "START_FRAME";
    case Operation::DRAW_WINDOW_FRAME: return "DRAW_WINDOW_FRAME";
    case Operation::DRAW: return "DRAW";
    case Operation::END_FRAME: return "END_FRAME";
    case Operation::COLLECT_MENU_KEYS: return "COLLECT_MENU_KEYS";
    case Operation::ESCAPE_KEY: return "ESCAPE_KEY";
    case Operation::BEGIN_START_UP: return "BEGIN_START_UP";
    case Operation::END_START_UP: return "END_START_UP";
    case Operation::RIGHT_MOUSE_CLICK: return "RIGHT_MOUSE_CLICK";
    case Operation::RIGHT_MOUSE_HOLD: return "RIGHT_MOUSE_HOLD";
    case Operation::LEFT_MOUSE_CLICK: return "LEFT_MOUSE_CLICK";
    case Operation::LEFT_MOUSE_HOLD: return "LEFT_MOUSE_HOLD";
    case Operation::LEFT_MOUSE_RELEASE: return "LEFT_MOUSE_RELEASE";
    case Operation::SCROLL: return "SCROLL";
    case Operation::W_KEY: return "W_KEY";
    case Operation::A_KEY: return "A_KEY";
    case Operation::S_KEY: return "S_KEY";
    case Operation::D_KEY: return "D_KEY";
    case Operation::G_KEY_PRESS: return "G_KEY_PRESS";
    case Operation::G_KEY_RELEASE: return "G_KEY_RELEASE";
    case Operation::CLICK_DEBUG_TIMER: return "CLICK_DEBUG_TIMER";
    case Operation::SHIFT_PRESS: return "SHIFT_PRESS";
    case Operation::SHIFT_RELEASE: return "SHIFT_RELEASE";
    default: return "";
    }
}