#pragma once
#include <stdint.h>
#include <libs/kernel/liballoc.h>
#include <libs/std/string.h>

template <class T>
class vector {
public:
    vector();
    void push_back(T item);
    T pop_back();
    void push_front(T item);
    T pop_front();
    uint32_t size();

     T & operator[](uint32_t index);

    //~vector();

private:
    T* _items;
    uint32_t itemCount = 0;
};

template<class T>
vector<T>::vector() {
    _items = (T*)malloc(sizeof(T));
}

// template<class T>
// vector<T>::~vector() {
//     free(_items);
//     itemCount = 0;
// }

template<class T>
void vector<T>::push_back(T item) {
    itemCount++;
    realloc(_items, itemCount * sizeof(T));
    _items[itemCount - 1] = item;
}

template<class T>
T vector<T>::pop_back() {
    if (itemCount == 0) {
        return NULL;
    }
    itemCount--;
    T item = _items[itemCount];
    realloc(_items, itemCount * sizeof(T));
    return item;
}

template<class T>
void vector<T>::push_front(T item) {
    itemCount++;
    realloc(_items, itemCount * sizeof(T));
    
    _items[0] = item;
}

template<class T>
T vector<T>::pop_front() {
    if (itemCount == 0) {
        return NULL;
    }
    itemCount--;
    T item = _items[0];
    memmove(&_items[0], &_items[1], itemCount * sizeof(T));
    realloc(_items, itemCount * sizeof(T));
    return item;
}

template<class T>
uint32_t vector<T>::size() {
    return itemCount;
}

template<class T>
T& vector<T>::operator[](uint32_t index) {
    return _items[index];
}