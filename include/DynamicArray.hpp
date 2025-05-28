#pragma once
#include "Exceptions.hpp"

template <typename T>
class DynamicArray {
private:
    T* items;
    int size;
    int capacity;  // для будущего)

public:
    DynamicArray() : items(nullptr), size(0), capacity(0) {}

    DynamicArray(const T* items, int count) {
        if (count < 0) {
            throw InvalidSizeException("Count cannot be negative");
        }
        this->size = count;
        this->capacity = count;
        this->items = new T[count];
        for (int i = 0; i < count; i++) {
            this->items[i] = items[i];
        }
    }
    DynamicArray(int size) : size(size), capacity(size) {
        if (size < 0) {
            throw InvalidSizeException("Size cannot be negative");
        }
        this->items = size > 0 ? new T[size]() : nullptr;
    }
    // from
    DynamicArray(const DynamicArray<T>& other) : size(other.size), capacity(other.size) {
        if (size == 0) {
            items = nullptr;
            return;
        }
        items = new T[size];
        for (int i = 0; i < size; ++i) {
            items[i] = other.items[i];
        }
    }

    DynamicArray& operator=(const DynamicArray<T>& other) {
        if (this != &other) {
            delete[] items;
            size = other.size;
            capacity = other.size;
            if (size == 0) {
                items = nullptr;
                return *this;
            }
            items = new T[size];
            for (int i = 0; i < size; ++i) {
                items[i] = other.items[i];
            }
        }
        return *this;
    }

    ~DynamicArray() {
        delete[] items;
    }

    T Get(int index) const {
        if (index < 0 || index >= size) {
            throw IndexOutOfRangeException("Index out of range");
        }
        return items[index];
    }

    int GetSize() const {
        return size;
    }

    void Set(int index, const T& value) {
        if (index < 0 || index >= size) {
            throw IndexOutOfRangeException("Index out of range");
        }
        items[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0) {
            throw InvalidSizeException("New size cannot be negative");
        }
        if (newSize == 0) {
            delete[] items;
            items = nullptr;
            size = 0;
            capacity = 0;
            return;
        }
        T* newItems = new T[newSize]();
        int copySize = newSize < size ? newSize : size;
        for (int i = 0; i < copySize; ++i) {
            newItems[i] = items[i];
        }
        delete[] items;
        items = newItems;
        size = newSize;
        capacity = newSize;
    }

    T& operator[](int index) {
        if (index < 0 || index >= size) {
            throw IndexOutOfRangeException("Index out of range");
        }
        return items[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw IndexOutOfRangeException("Index out of range");
        }
        return items[index];
    }
};