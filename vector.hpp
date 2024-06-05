#ifndef VECTOR_HPP
#define VECTOR_HPP
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
        if (index < current) {
            return arr[index];
        }
        throw out_of_range("Vector index out of range");
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

    T get(int index) const {
        if (index < current) {
            return arr[index];
        }
        return T();
    }

    int size() const {
        return current;
    }

    ~Vector() {
        delete[] arr;
    }
};
#endif