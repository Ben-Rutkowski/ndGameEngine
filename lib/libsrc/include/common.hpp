#ifndef COMMON_HPP
#define COMMON_HPP

// === Enums ===

enum class EventType { 
    null = 0, ACTION, LEN
};

enum class Module { 
    null = 0, TEST, APPLICATION, WINDOW, LEN
};

enum class Data { 
    null = 0, TEST, CLOSE_APP, ESCAPE_KEY, LEN
};

#define EVENT_DATA_LEN (int)Data::LEN

#endif