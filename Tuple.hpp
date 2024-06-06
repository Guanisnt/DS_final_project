#ifndef TUPLA_HPP
#define TUPLA_HPP
template <class T>
class Tuple {
public:
    T data[4];
    T a, b, c, d;
    Tuple() : a(), b(), c(), d() {}
    Tuple(T a, T b, T c, T d) {
        data[0] = a;
        data[1] = b;
        data[2] = c;
        data[3] = d;
    }

    bool operator==(const Tuple& other) const {
        for (int i = 0; i < 4; i++) {
            if (data[i] != other.data[i]) {
                return false;
            }
        }
        return true;
    }
};
#endif