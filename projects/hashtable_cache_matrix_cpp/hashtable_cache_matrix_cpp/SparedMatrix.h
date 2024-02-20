#pragma once

#include "HashTable.h"

class pair_hash {
public:
    unsigned int operator()(const Pair<unsigned int, unsigned int>& p) const {
        return (unsigned int) (p.get_first() * p.get_first() + sqrt(p.get_second()));
    }
};

template <typename V, typename F = pair_hash>
class SparedMatrix {
private:
    HashTable<Pair<unsigned int, unsigned int>, V, F> matrix;
    Pair<unsigned int, unsigned int> dimension;

public:
    SparedMatrix() : matrix(), dimension() {}
    SparedMatrix(unsigned int x, unsigned int y) : matrix(x*y/10 > 0 ? x*y/10 : 1), dimension(x, y) {}
    explicit SparedMatrix(const SparedMatrix<V> &a) : matrix(a), dimension(a.dimension) {}

    bool insert(unsigned int x, unsigned int y, const V& value);

    std::unique_ptr<SparedMatrix<V, F>> multiply(SparedMatrix<V> &other);
    std::unique_ptr<SparedMatrix<V, F>> add(SparedMatrix<V> &other);

    std::unique_ptr<SparedMatrix<V, F>> map(std::function<V(const V&)> func);
    void reduce(std::function<V(const V&, const V&)> func, V& value);

    [[nodiscard]] unsigned int x_scale() const { return dimension.get_first(); }
    [[nodiscard]] unsigned int y_scale() const { return dimension.get_second(); }

    void print();
    void print2() { matrix.print(); }

    bool resize() { return matrix.rehash_table(); }
    unsigned int get_size() { return matrix.get_size(); }
    unsigned int get_elem_number() { return matrix.get_elem_number(); }
    unsigned int get_max_bucket_height() { return matrix.get_max_bucket_height(); }
};

template<typename V, typename F>
std::unique_ptr<SparedMatrix<V, F>> SparedMatrix<V, F>::multiply(SparedMatrix<V> &other) {
    if (this->y_scale() != other.x_scale())
        throw std::runtime_error("\n[SparedMatrix] multiply message: matrices can't be multiplied\n");
    std::unique_ptr<SparedMatrix<V>> res(new SparedMatrix<V>(this->y_scale(), other.x_scale()));
    for (auto &a: this->matrix) {
        for (auto &b: other.matrix) {
            if (a.get_first().get_first() == b.get_first().get_second()) {
                Pair<unsigned int, unsigned int> p(b.get_first().get_first(), a.get_first().get_second());
                auto tmp = res->matrix.get_(p);
                V new_value;
                if (tmp != res->matrix.end()) {
                    new_value = (*tmp).get_second() + (a.get_second() * b.get_second());
                } else {
                    new_value = a.get_second() * b.get_second();
                } if (new_value == V()) {
                    res->matrix.remove(p);
                } else {
                    res->matrix.insert(p, new_value);
                }
            }
        }
    }
    return res;
}

template<typename V, typename F>
std::unique_ptr<SparedMatrix<V, F>> SparedMatrix<V, F>::add(SparedMatrix<V> &other) {
    if ((this->x_scale() != other.x_scale()) || (this->y_scale() != other.y_scale()))
        throw std::runtime_error("\n[SparedMatrix] add message: matrices can't be added\n");
    std::unique_ptr<SparedMatrix<V>> res(new SparedMatrix<V>(this->x_scale(), this->y_scale()));
    for (auto &a: this->matrix) {
        Pair<unsigned int, unsigned int> p(a.get_first().get_first(), a.get_first().get_second());
        auto it = other.matrix.get_(p);
        V new_value;
        if (it != other.matrix.end()) {
            new_value = a.get_second() + (*it).get_second();
            if (new_value != V())
                res->matrix.insert(p, new_value);
        }
        else
            res->matrix.insert(p, a.get_second());
    }
    for (auto &b: other.matrix) {
        Pair<unsigned int, unsigned int> p(b.get_first().get_first(), b.get_first().get_second());
        auto it = this->matrix.get_(p);
        if (it == this->matrix.end())
            res->matrix.insert(p, b.get_second());
    }
    return res;
}

template<typename V, typename F>
bool SparedMatrix<V, F>::insert(unsigned int x, unsigned int y, const V &value) {
    if (value == V())
        throw std::runtime_error("\n[SparedMatrix] insert message: you have tried to insert zero\n");
    if ((x >= x_scale()) || (y >= y_scale()))
        throw std::runtime_error("\n[SparedMatrix] insert message: out of bounds\n");
    Pair<unsigned int, unsigned int> p(x, y);
    return matrix.insert(p, value);
}

template<typename V, typename F>
void SparedMatrix<V, F>::print() {
    for (int i = 0; i < y_scale(); ++i) {
        for (int j = 0; j < x_scale(); ++j) {
            Pair<unsigned int, unsigned int> p(j, i);
            auto f = matrix.get_(p);
            if (f != matrix.end()) std::cout << (*f).get_second() << " ";
            else std::cout << V() << " ";
        }
        std::cout << "\n";
    }
}

template<typename V, typename F>
std::unique_ptr<SparedMatrix<V, F>> SparedMatrix<V, F>::map(std::function<V(const V&)> func) {
    std::unique_ptr<SparedMatrix<V>> res(new SparedMatrix<V>(this->x_scale(), this->y_scale()));
    for (auto &a: this->matrix) {
        Pair<unsigned int, unsigned int> p(a.get_first().get_first(), a.get_first().get_second());
        V new_value = func(a.get_second());
        res->matrix.insert(p, new_value);
    }
    return res;
}

template<typename V, typename F>
void SparedMatrix<V, F>::reduce(std::function<V(const V&, const V&)> func, V& value) {
    value = V();
    for (auto &a: this->matrix) {
        value = func(value, a.get_second());
    }
}
