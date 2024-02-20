#pragma once

template <typename T>
class Iterator {
public:
    virtual T& operator*() = 0;
    virtual Iterator<T>& operator++() = 0;
    virtual Iterator<T>& operator--() = 0;
    virtual std::unique_ptr<Iterator<T>> operator+(int x) = 0;
    virtual std::unique_ptr<Iterator<T>> operator-(int x) = 0;
    virtual std::unique_ptr<Iterator<T>> copy() = 0;
    virtual int get_index() const = 0;
};

template <typename T>
bool operator==(const Iterator<T>& it1, const Iterator<T>& it2) {
    return it1.get_index() == it2.get_index();
};

template <typename T>
bool operator!=(const Iterator<T>& it1, const Iterator<T>& it2) {
    return it1.get_index() != it2.get_index();
};

template <typename T>
int operator-(const Iterator<T>& it1, const Iterator<T>& it2) {
    return it1.get_index() - it2.get_index();
};

template <typename T>
bool operator<(const Iterator<T>& it1, const Iterator<T>& it2) {
    return it1.get_index() < it2.get_index();
};

template <typename T>
bool operator>(const Iterator<T>& it1, const Iterator<T>& it2) {
    return it1.get_index() > it2.get_index();
};

template <typename T>
bool operator<=(const Iterator<T>& it1, const Iterator<T>& it2) {
    return it1.get_index() <= it2.get_index();
};

template <typename T>
bool operator>=(const Iterator<T>& it1, const Iterator<T>& it2) {
    return it1.get_index() >= it2.get_index();
};