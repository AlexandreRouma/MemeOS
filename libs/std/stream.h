#pragma once
#include <stdint.h>
#include <libs/kernel/liballoc.h>
#include <libs/std/string.h>

template <class T>
class stream {
public:
    stream();
    stream(void (*handler)(T*), int buffer_size = 1);
    int read(T* buf, int size);
    int write(T* buf, int size);
    void flush();

private:
    T _item_unb;
    T* _items;
    uint32_t itemCount = 0;
    uint32_t bufferSize = 0;
    void (*_handler)(T*);
};

template<class T>
stream<T>::stream() {
    bufferSize = 0;
}

template<class T>
stream<T>::stream(void (*handler)(T*), int buffer_size) {
    bufferSize = buffer_size;
    _handler = handler;
    if (buffer_size <= 1) {
        _items = &_item_unb;
    }
    else {
        _items = (T*)malloc(buffer_size);
    }
}

template<class T>
int stream<T>::read(T* buf, int size) {
    if (size == 0 || bufferSize == 0) {
        return -1;
    }
    if (size > itemCount) {
        size = itemCount;
    }
    memmove(buf, _items, size);
    return size;
}

template<class T>
int stream<T>::write(T* buf, int size) {
    if (size <= 0 || bufferSize == 0) {
        return -1;
    }
    for (int i = 0; i < size; i++) {
        _items[itemCount] = buf[i];
        itemCount++;
        if (itemCount == bufferSize) {
            flush();
        }
    }
    return size;
}

template<class T>
void stream<T>::flush() {
    if (bufferSize == 0) {
        return;
    }
    itemCount = 0;
    _handler(_items);
}