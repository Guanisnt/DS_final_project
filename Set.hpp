#ifndef SET_HPP
#define SET_HPP
#include "Vector.hpp"
template <class T>
class Set {
private:
    Vector<T> data;

public:
    void insert(const T& value) {
        for (int i = 0; i < data.size(); i++) {
            if (data.get(i) == value) {
                return;
            }
        }
        data.push_back(value);
    }

    int size() const {
        return data.size();
    }

    typename Vector<T>::Iterator find(const T& value) {
        return data.find(value);
    }
    
    typename Vector<T>::Iterator end() {
        return data.end();
    }
};
#endif