#ifndef VECTOR_HPP
#define VECTOR_HPP
template <typename T>
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

    ~Vector() {
        delete[] arr;
    }
};
#endif