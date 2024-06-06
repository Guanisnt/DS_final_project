#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <iostream>
using namespace std;
template <class T>
class Vector {
private:
    T* arr;
    int capacity;
    int current;

public:
    Vector() {
        arr = new T[1];
        capacity = 1;
        current = 0;
    }

    Vector(int size, T data) {
        arr = new T[size];
        capacity = size;
        current = size;

        for (int i = 0; i < size; i++) {
            arr[i] = data;
        }
    }

    T& at(int index) {
        if (index < 0 || index >= current) {
            throw out_of_range("Vector index out of range");
        }
        return arr[index];
    }


    void push_back(T data) {
        if (current == capacity) {
            T* temp = new T[2 * capacity];

            for (int i = 0; i < capacity; i++) {
                temp[i] = arr[i];
            }

            delete[] arr;
            capacity *= 2;
            arr = temp;
        }

        arr[current] = data;
        current++;
    }

    void pop_back() {
        if (current > 0) {
            current--;
        }
    }

    T get(int index) const {
        if (index < 0 || index >= current) {
            return T();
        }
        return arr[index];
    }

    int size() const {
        return current;
    }

    bool empty() const {
        return current == 0;
    }

    T front() const {
        if (current > 0) {
            return arr[0];
        }
        return T();
    }

    T back() const {
        if (current > 0) {
            return arr[current - 1];
        }
        return T();
    }

    ~Vector() {
        delete[] arr;
    }
};
#endif