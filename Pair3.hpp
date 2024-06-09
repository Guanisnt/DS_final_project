#ifndef PAIR3_HPP
#define PAIR3_HPP
#include <string>
using namespace std;
template <typename T1, typename T2, typename T3>
class Pair3 {
public:
    T1 first;
    T2 second;
    T3 third;

    Pair3(const T1& first, const T2& second, const T3& third) : first(first), second(second), third(third) {}
    Pair3() : first(""), second(""), third("") {}

    bool operator<(const Pair3& other) const {
        if (first == other.first) {
            if (second == other.second) {
                return third < other.third;
            } else {
                return second < other.second;
            }
        } else {
            return first < other.first;
        }
    }

    bool operator==(const Pair3& other) const {
        return first == other.first && second == other.second && third == other.third;
    }
};
#endif