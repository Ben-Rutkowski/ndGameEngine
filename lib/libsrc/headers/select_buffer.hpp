#ifndef SELECT_BUFFER_HPP
#define SELECT_BUFFER_HPP

#include <vector>
#include <stdint.h>

template<typename T>
struct SelectBuffer {
private:
    T*  buffer;
    int buffer_max_size;
    int buffer_end;
    std::vector<uint32_t> buffer_meta;

    T*  select;
    int select_max_size;
    int select_end;
    std::vector<uint32_t> select_meta;

public:
    SelectBuffer()
        :buffer_end{0}, select_end{0} {}

    void linkBuffers(
        T* buffer_ptr_in, int buffer_max_size_in, 
        T* select_ptr_in, int select_max_size_in
    )
    {
        buffer = buffer_ptr_in;
        buffer_max_size = buffer_max_size_in;
        buffer_meta.reserve(buffer_max_size);

        select = select_ptr_in;
        select_max_size = select_max_size_in;
        select_meta.reserve(select_max_size);
    }

// --- Access ---
public:
    T& operator[](const int id) { return buffer[id]; }
    T& getSelect(const int id)  { return select[id]; }

// --- Operations ---
public:
    uint32_t addPoint(const T point) {
        buffer[buffer_end] = point;
        buffer_meta[buffer_end] = 1;
        return ++buffer_end;
    }

    uint32_t deletePoint(const uint32_t d_pnt_id) {
        T point = buffer[buffer_end-1];
        buffer[d_pnt_id] = point;
        return --buffer_end;
    }

    void selectPoint(const uint32_t s_pnt_id) {
        T point = buffer[s_pnt_id];
        buffer_meta[s_pnt_id] = 0;

        select[select_end] = point;
        select_meta[select_end] = s_pnt_id;
        ++select_end;
    }

    void flushSelect() {
        T point;
        uint32_t pnt_id;

        for (int i=0; i<select_end; i++) {
            point  = select[select_end-1];
            pnt_id = select_meta[select_end-1];

            buffer[pnt_id] = point;
            buffer_meta[pnt_id] = 1;
            select_end--;
        }
    }

// --- Debug ---
public:
    void debug() {
        printf("BUFFER:\n");
        for (int i=0; i<buffer_end; i++) {
            printf("%d: ", i);
            buffer[i].debug();
            printf(" | %d", buffer_meta[i]);
            printf("\n");
        }

        printf("SELECT:\n");
        for (int i=0; i<select_end; i++) {
            printf("%d: ", i);
            select[i].debug();
            printf(" | %d", select_meta[i]);
            printf("\n");
        }

        printf("\n");
    }
};  

#endif
