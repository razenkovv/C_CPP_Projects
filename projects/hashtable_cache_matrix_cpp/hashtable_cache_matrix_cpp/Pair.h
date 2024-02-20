#pragma once

#include <iostream>

template <typename T1, typename T2>
class Pair;

template<typename T1, typename T2>
std::ostream &operator<< (std::ostream &, const Pair<T1, T2> &);

template<typename T1, typename T2>
bool operator== (const Pair<T1, T2>& p1, const Pair<T1, T2>& p2);

template <typename T1, typename T2>
class Pair {
private:
    T1 object1;
    T2 object2;
public:
    Pair() : object1(), object2() {}
    Pair(const T1& obj1, const T2& obj2);
    Pair(const Pair<T1, T2> &other_pair);

    const T1& get_first() const { return object1; }
    const T2& get_second() const { return object2; }

    void set_first(const T1& value1) { object1 = std::move(value1); }
    void set_second(const T2& value2) { object2 = std::move(value2); }

    void print();
    friend std::ostream &operator<< <T1, T2>(std::ostream &out, const Pair<T1, T2> &p);
    friend bool operator== <T1, T2>(const Pair<T1, T2>& p1, const Pair<T1, T2>& p2);
};

template<typename T1, typename T2>
Pair<T1, T2>::Pair(const T1 &obj1, const T2 &obj2) {
    new(&object1) T1(obj1);
    new(&object2) T2(obj2);
}

template<typename T1, typename T2>
Pair<T1, T2>::Pair(const Pair<T1, T2> &other_pair) {
    new(&object1) T1(other_pair.get_first());
    new(&object2) T2(other_pair.get_second());
}

template<typename T1, typename T2>
void Pair<T1, T2>::print() {
    std::cout << object1 << " " << object2 << "\n";
}

template<typename T1, typename T2>
std::ostream &operator<<(std::ostream &out, const Pair<T1, T2> &p) {
    out << p.object1 << " " << p.object2;
    return out;
}

template<typename T1, typename T2>
bool operator== (const Pair<T1, T2>& p1, const Pair<T1, T2>& p2) {
    return (p1.get_first() == p2.get_first()) && (p1.get_second() == p2.get_second());
}