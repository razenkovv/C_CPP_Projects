#pragma once

#include <cstdint>
#include <memory>

#include "Iterator.h"

template <typename T>
class Array_Iterator;

template <typename T>
class Vector {
    friend class Array_Iterator<T>;
private:
    T *m_array;
    unsigned int m_size;
    unsigned int m_cap;

public:
    Array_Iterator<T> begin() { return Array_Iterator<T>(this, m_array, 0); }
    Array_Iterator<T> end() { return Array_Iterator<T>(this, m_array + this->size(), this->size()); }
    Array_Iterator<T> last() { return Array_Iterator<T>(this, m_array + (this->size() - 1), this->size() - 1); }

    Vector() : m_size(0), m_cap(0) { m_array = nullptr; } //создание вектора по умолчанию

    explicit Vector(unsigned int size); //создание вектора длины size

    explicit Vector(unsigned int size, const T &value); //создание вектора длины size и заполнение его значением value

    Vector(unsigned int number, T* items); //скопировать number элемементов из переданного массива items

    Vector(const Vector<T> &other_vector); //конструктор копирования

    ~Vector(); //деструктор

    int size() const { return m_size; } //возвращает длину вектора

    int capacity() const { return m_cap; } //возвращает вместимость вектора

    void reserve(int new_cap); //делает вместимость вектора равной new_cap элементов

    void resize(int new_size);

    void resize(int new_size, const T &value); //делает длину вектора равной new_size элементов, заполняет новые элементы значением value

    void shrink_to_fit(); //уменьшение m_cap до m_size

    T& get(int index); //получение элемента с номером index

    T& front(); //возращает первый элемент

    T& back(); //возвращает последний элемент

    void set(int index, const T &value = T()); //устаноить новое значение value по индексу i

    void push_back(const T &value); //добавление элемента в конец

    void push_front(const T &value); //добавление элемента в начало

    void pop_back(); //уничтожение последнего элемента

    void clear(); //очистка вектора

    void print(); //вывод на экран

    T& operator[] (int i); //перегрузка оператора []
};



template <typename T>
class Array_Iterator : public Iterator<T> {
    friend class Vector<T>;

protected:
    Vector<T>* m_array;
    T* m_ptr;
    int m_index;

public:
    Array_Iterator() : m_array(nullptr), m_ptr(nullptr), m_index(0) {};

    Array_Iterator(Vector<T>* array, T* ptr, int index) : m_array(array), m_ptr(ptr), m_index(index) {};

    T& operator*() override {
        if (*this == m_array->end())
            throw std::runtime_error("\n(Array_Iterator) operator *: index out of range\n");
        return *m_ptr;
    }

    Array_Iterator& operator++() override {
        if (*this == m_array->end())
            throw std::runtime_error("\n(Array_Iterator) operator ++: index out of range\n");
        ++m_ptr;
        ++m_index;
        return *this;
    }

    Array_Iterator& operator--() override{
        if (*this == m_array->begin())
            throw std::runtime_error("\n(Array_Iterator) operator --: index out of range\n");
        --m_ptr;
        --m_index;
        return *this;
    }

    std::unique_ptr<Iterator<T>> operator+(int x) override{
        if (((m_index + x) < 0) || ((m_index + x) >= m_array->size()))
            throw std::runtime_error("\n(Array_Iterator) operator +: index out of range\n");
        std::unique_ptr<Array_Iterator<T>> res(new Array_Iterator<T>(m_array, m_ptr + x, m_index + x));
        return res;
    }

    std::unique_ptr<Iterator<T>> operator-(int x) override{
        if (((m_index - x) < 0) || ((m_index - x) > m_array->size()))
            throw std::runtime_error("\n(Array_Iterator) operator -: index out of range\n");
        std::unique_ptr<Array_Iterator<T>> res(new Array_Iterator<T>(m_array, m_ptr - x, m_index - x));
        return res;
    }

    std::unique_ptr<Iterator<T>> copy() override{
        std::unique_ptr<Array_Iterator<T>> res(new Array_Iterator<T>(m_array, m_ptr, m_index));
        return res;
    }

    int get_index() const override{ return m_index; }

    friend bool operator== (const Array_Iterator &it1, const Array_Iterator &it2) { return it1.m_ptr == it2.m_ptr; }
    friend bool operator!= (const Array_Iterator &it1, const Array_Iterator &it2) { return it1.m_ptr != it2.m_ptr; }
    friend bool operator< (const Array_Iterator &it1, const Array_Iterator &it2) { return it1.m_index < it2.m_index; }
    friend bool operator> (const Array_Iterator &it1, const Array_Iterator &it2) { return it1.m_index > it2.m_index; }
    friend bool operator<= (const Array_Iterator &it1, const Array_Iterator &it2) { return it1.m_index <= it2.m_index; }
    friend bool operator>= (const Array_Iterator &it1, const Array_Iterator &it2) { return it1.m_index >= it2.m_index; }

    friend int operator- (const Array_Iterator &it1, const Array_Iterator &it2) { return it1.m_ptr - it2.m_ptr; }
};



template <typename T>
Vector<T>::~Vector() {
    clear();
}

template <typename T>
Vector<T>::Vector(unsigned int size) {
    m_array = reinterpret_cast<T*>(new int8_t[size * sizeof(T)]);
    m_size = size;
    m_cap = size;
    if (size == 0) m_array = nullptr;
}

template <typename T>
Vector<T>::Vector(unsigned int size, const T &value) {
    m_array = reinterpret_cast<T*>(new int8_t[size * sizeof(T)]);
    m_size = size;
    m_cap = size;
    for (int i = 0; i < m_size; ++i) {
        new(m_array + i) T(value);
    }
    if (size == 0) m_array = nullptr;
}

template <typename T>
Vector<T>::Vector(unsigned int number, T* items) {
    if (items == nullptr) {
        m_array = nullptr;
        m_size = 0;
        m_cap = 0;
    } else {
        m_size = number;
        m_cap = number;
        m_array = reinterpret_cast<T *>(new int8_t[number * sizeof(T)]);
        for (int i = 0; i < m_size; ++i) {
            new(m_array + i) T(items[i]);
        }
    }
}

template<typename T>
Vector<T>::Vector(const Vector<T> &other_vector) {
    m_cap = other_vector.m_cap;
    m_size = other_vector.m_size;
    m_array = reinterpret_cast<T *>(new int8_t[m_cap * sizeof(T)]);
    for (int i = 0; i < m_size; ++i) {
        new(m_array + i) T(other_vector.m_array[i]);
    }
}

template <typename T>
void Vector<T>::clear() {
    for (int i = 0; i < m_size; ++i) {
        (m_array + i)->~T();
    }
    delete[] reinterpret_cast<int8_t*>(m_array);
    m_array = nullptr;
    m_size = 0;
    m_cap = 0;
}

template <typename T>
void Vector<T>::reserve(int new_cap) {
    if (new_cap < 0)
        throw std::runtime_error("\n(Vector) Reserve Message: new_cap < 0\n");
    if (new_cap <= m_cap) return;
    T *new_arr = reinterpret_cast<T*>(new int8_t[new_cap * sizeof(T)]);
    for (int i = 0; i < m_size; ++i) {
        new(new_arr + i) T(m_array[i]); //placement new
    }
    for (int i = 0; i < m_size; ++i) {
        (m_array + i)->~T();
    }
    delete[] reinterpret_cast<int8_t*>(m_array);
    m_array = new_arr;
    m_cap = new_cap;
}

template <typename T>
void Vector<T>::resize(int new_size) {
    if (new_size < 0)
        throw std::runtime_error("\n(Vector) Resize Message: new_size < 0\n");
    if (new_size > m_cap) reserve(new_size);
    if (new_size < m_size) {
        for (int i = new_size; i < m_size; ++i) {
            (m_array + i)->~T();
        }
    }
    m_size = new_size;
}

template <typename T>
void Vector<T>::resize(int new_size, const T &value) {
    if (new_size < 0)
        throw std::runtime_error("\n(Vector) Resize Message: new_size < 0\n");
    if (new_size > m_cap) reserve(new_size);
    if (new_size < m_size) {
        for (int i = new_size; i < m_size; ++i) {
            (m_array + i)->~T();
        }
    } else {
        for (int i = m_size; i < new_size; ++i) {
            new(m_array + i) T(value);
        }
    }
    m_size = new_size;
}

template <typename T>
void Vector<T>::shrink_to_fit() {
    T *new_arr = reinterpret_cast<T*>(new int8_t[m_size * sizeof(T)]);
    for (int i = 0; i < m_size; ++i) {
        new(new_arr + i) T(m_array[i]);
    }
    for (int i = 0; i < m_size; ++i) {
        (m_array + i)->~T();
    }
    delete[] reinterpret_cast<int8_t *>(m_array);
    m_array = new_arr;
    m_cap = m_size;
}

template <typename T>
T& Vector<T>::get(int i) {
    if ((i < 0) || (i >= m_size))
        throw std::runtime_error("\n(Vector) Get Message : Index Out Of Range\n");
    return m_array[i];
}

template <typename T>
T& Vector<T>::front() {
    if (m_size == 0)
        throw std::runtime_error("\n(Vector) Front Message : Vector is empty\n");
    return m_array[0];
}

template <typename T>
T& Vector<T>::back() {
    if (m_size == 0)
        throw std::runtime_error("\n(Vector) Back Message : Vector is empty\n");
    return m_array[m_size - 1];
}


template <typename T>
void Vector<T>::set(int i, const T &value) {
    if ((i < 0) || (i >= m_size))
        throw std::runtime_error("\n(Vector) Set Message : Index Out Of Range\n");
    new(m_array+i) T(value);
}

template <typename T>
void Vector<T>::push_back(const T &value) {
    resize(m_size + 1, value);
}

template <typename T>
void Vector<T>::push_front(const T &value) {
    if ((m_size + 1) > m_cap) reserve(m_size + 1);
    T *new_arr = reinterpret_cast<T*>(new int8_t[m_cap * sizeof(T)]);
    new(new_arr) T(value);
    for (int i = 1; i < (m_size + 1); ++i) {
        new(new_arr + i) T(m_array[i - 1]);
    }
    for (int i = 0; i < m_size; ++i) {
        (m_array + i)->~T();
    }
    delete[] reinterpret_cast<int8_t *>(m_array);
    m_array = new_arr;
    ++m_size;
}

template <typename T>
void Vector<T>::pop_back() {
    if (m_size == 0)
        throw std::runtime_error("\n(Vector) Pop_back Message: size = 0\n");
    resize(m_size - 1);
}

template <typename T>
T& Vector<T>::operator[] (int i) {
    if ((i < 0) || (i >= m_size))
        throw std::runtime_error("\n(Vector) Operator [] Message: Index Out Of Range\n");
    return m_array[i];
}

template<typename T>
void Vector<T>::print() {
    for (int i = 0; i < m_size; ++i) {
        std::cout << get(i) << " ";
    }
    std::cout << std::endl;
}