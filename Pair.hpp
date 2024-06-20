#ifndef PAIR_HPP
#define PAIR_HPP
#include <string>
using namespace std;
template <typename T1, typename T2>
class Pair {
public:
    T1 first;
    T2 second;

    Pair(const T1& first, const T2& second) : first(first), second(second) {}
    // Pair() : first(""), second("") {}
    Pair() = default;

    bool operator<(const Pair& other) const {
        if (first == other.first) {
            return second < other.second;
        } else {
            return first < other.first;
        }
    }

    bool operator==(const Pair& other) const {
        return first == other.first && second == other.second;
    }
};
#endif