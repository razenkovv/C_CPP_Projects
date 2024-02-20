#pragma once

#include "Vector.h"
#include "List.h"
#include <memory>

template <typename T>
class Sequence {
public:

    virtual void concat(Sequence<T> &other_seq) = 0;
    virtual std::unique_ptr<Sequence<T>> get_subsequence(int start, int end) = 0;

    virtual T& get_first() = 0;
    virtual T& get_last() = 0;
    virtual T& get(int index) = 0;
    virtual int size() = 0;

    virtual void push_front(const T& value) = 0;
    virtual void push_back(const T& value) = 0;
    virtual void insert_at(int index, const T& value) = 0;
    virtual void set(int index, const T& value) = 0;

    virtual void print() = 0;

    virtual ~Sequence() = default;
};



template<typename T>
class ArraySequence : public Sequence<T> {
private:
    Vector<T> array_data;

public:
    Array_Iterator<T> begin() { return array_data.begin(); }
    Array_Iterator<T> end() { return array_data.end(); }
    Array_Iterator<T> last() { return array_data.last(); }

    ArraySequence() : array_data() {} //конструктор по умолчанию

    explicit ArraySequence(unsigned int size, const T &value=T()) : array_data(size, value) {} //создание sequence длины size и заполнение её значением value

    ArraySequence(unsigned int number, T* items) : array_data(number, items) {} //скопировать number элемементов из переданного массива items

    explicit ArraySequence(const Sequence<T> &other_seq) : array_data(other_seq.array_data) {} //конструктор копирования

    explicit ArraySequence(const Vector<T> &other_vector) : array_data(other_vector) {}

    ~ArraySequence() = default;

    T& get_first() { return array_data.front(); }

    T& get_last() { return array_data.back(); }

    T& get(int index) { return array_data.get(index); }

    std::unique_ptr<Sequence<T>> get_subsequence(int start, int end);

    std::unique_ptr<ArraySequence<T>> copy();

    void concat(Sequence<T> &other_seq);

    void insert_at(int index, const T& value);

    int size() { return array_data.size(); }

    void set(int index, const T &value = T()) { array_data.set(index, value); }

    void push_front(const T &value){ array_data.push_front(value); }

    void push_back(const T &value){ array_data.push_back(value); }

    void print() { array_data.print(); }
};

template<typename T>
std::unique_ptr<Sequence<T>> ArraySequence<T>::get_subsequence(int start, int end) {
    if ((start > end) || (start >= array_data.size()) || (end >= array_data.size()) || (start < 0) || (end < 0))
        throw std::runtime_error("\n(ArraySequence) Get_subsequence Message : Index Out Of Range\n");
    std::unique_ptr<ArraySequence<T>> res(new ArraySequence<T>{static_cast<unsigned int> (end - start + 1)});
    for (int i = 0; i <= (end - start); ++i) {
        new(&(res->array_data.get(i))) T(array_data.get(i + start));
    }
    return res;
}

template<typename T>
std::unique_ptr<ArraySequence<T>> ArraySequence<T>::copy() {
    std::unique_ptr<ArraySequence<T>> res(new ArraySequence<T>{static_cast<unsigned int> (this->size())});
    for (int i = 0; i < this->size(); ++i) {
        new(&(res->array_data.get(i))) T(array_data.get(i));
    }
    return res;
}

template<typename T>
void ArraySequence<T>::concat(Sequence<T> &other_seq) {
    unsigned int tmp_size = array_data.size();
    array_data.resize(array_data.size() + other_seq.size());
    for (int i = 0; i < other_seq.size(); ++i) {
        new(&(array_data.get(i + tmp_size))) T(other_seq.get(i));
    }
}

template<typename T>
void ArraySequence<T>::insert_at(int index, const T &value) {
    if ((index < 0) || (index >= array_data.size()))
        throw std::runtime_error("\n(ArraySequence) Insert_at Message : Index Out Of Range\n");
    array_data.resize(array_data.size() + 1);
    for (int i = array_data.size() - 2; i >= index ; i--) {
        array_data.set(i + 1, array_data.get(i));
    }
    array_data.set(index, value);
}


template<typename T>
class ListSequence : public Sequence<T> {
private:
    List<T> list_data;
public:
    List_Iterator<T> begin() { return list_data.begin(); }
    List_Iterator<T> end() { return list_data.end(); }
    List_Iterator<T> last() { return list_data.last(); }

    ListSequence() : list_data() {} //конструктор по умолчанию

    explicit ListSequence(unsigned int size, const T &value=T()) : list_data(size, value) {} //создание sequence длины size и заполнение её значением value

    ListSequence(unsigned int number, T* items) : list_data(number, items) {} //скопировать number элемементов из переданного массива items

    explicit ListSequence(const Sequence<T> &other_seq) : list_data(other_seq.list_data) {} //конструктор копирования

    explicit ListSequence(const List<T> &other_list) : list_data(other_list) {}

    ~ListSequence() = default;

    T& get_first() { return list_data.front(); }

    T& get_last() { return list_data.back(); }

    T& get(int index) { return list_data.get(index); }

    std::unique_ptr<Sequence<T>> get_subsequence(int start, int end);

    void concat(Sequence<T> &other_seq);

    void insert_at(int index, const T& value) {list_data.insert_at(index, value); }

    int size() { return list_data.size(); }

    void set(int index, const T &value = T()) { list_data.set(index, value); }

    void push_front(const T &value){ list_data.push_front(value); }

    void push_back(const T &value){ list_data.push_back(value); }

    void print() { list_data.print(); }
};

template<typename T>
std::unique_ptr<Sequence<T>> ListSequence<T>::get_subsequence(int start, int end) {
    if ((start > end) || (start >= list_data.size()) || (end >= list_data.size()) || (start < 0) || (end < 0))
        throw std::runtime_error("\n(ListSequence) Get_subsequence Message : Index Out Of Range\n");
    std::unique_ptr<ListSequence<T>> res(new ListSequence<T>{*(list_data.get_sublist(start, end))});
    return res;
}

template<typename T>
void ListSequence<T>::concat(Sequence<T> &other_seq) {
    for (int i = 0; i < other_seq.size(); ++i) {
        list_data.push_back(other_seq.get(i));
    }
}