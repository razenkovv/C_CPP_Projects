#pragma once
#include <functional>

#include "Sequences.h"
#include "Pair.h"

//K - key, V - value, F - hash_function
template <typename K, typename V, typename F>
class Hash_Iterator;

template <typename K>
class HashFunctions {
public:
    unsigned int operator()(const unsigned int x) const {
        return x;
    }

    unsigned int operator()(const double x) const {
        return (unsigned int) x;
    }
};

template <typename K, typename V, typename F = HashFunctions<K>>
class HashTable {
protected:
    ArraySequence<ListSequence<Pair<K, V>>> table;
    unsigned int table_size; //размер таблицы
    unsigned int elem_number; //число элементов в таблице
    double load_factor; //отношение elem_number к table_size
    static constexpr unsigned int q_factor = 2;
    static constexpr double rehash_factor_1 = 0.75;
    static constexpr double rehash_factor_2 = 0.1;

    F hash_function;
    unsigned int hash_func(const K& key) { unsigned int res = hash_function(key) * table_size * 1.618; return res % table_size; }

    void rehash_table(double k); //увеличение(уменьшение) размера хеш-таблицы в q_factor раз при достижении load_factor значений rehash_factor.

public:
    HashTable() : table(), table_size(0), elem_number(0), load_factor(0) {}
    explicit HashTable(unsigned int n) : table(n, ListSequence<Pair<K, V>>()), table_size(n), elem_number(0), load_factor(0) {}
    HashTable(const HashTable<K, V, F> &a) : table(a.table), table_size(a.table_size), elem_number(a.elem_number), load_factor(a.load_factor) {}
    ~HashTable();

    bool insert(const K& key, const V& value); //вставка нового ключа. если он уже есть - значение изменяется
    bool remove(const K& key); //удаление ключа. если его нет, возвращается false
    bool contain(const K& key); //возвращает true если ключ есть, false - если ключа нет
    bool get(const K& key, V& value); //в value будет записано значение, найденное по ключу (если ключ есть, иначе возвращается false, причем value не изменяется)

    Hash_Iterator<K, V, F> get_(const K& key); //тоже get, но вовзращает итератор. если ключ не найден, вернется past-the-end итератор

    unsigned int get_size() { return table_size; }
    unsigned int get_elem_number() { return elem_number; }
    unsigned int get_max_bucket_height();

    void print();

    ArraySequence<ListSequence<Pair<K, V>>>* get_array() { return &table; }

    Hash_Iterator<K, V, F> begin();
    Hash_Iterator<K, V, F> end();
};

template <typename K, typename V, typename F>
class Hash_Iterator : public HashTable<K, V, F> {
protected:
    HashTable<K, V, F>* m_table;
    List_Iterator<Pair<K, V>> list_it;
    Array_Iterator<ListSequence<Pair<K, V>>> array_it;

public:
    Hash_Iterator(HashTable<K, V, F>* table, List_Iterator<Pair<K, V>> list_it_,
                        Array_Iterator<ListSequence<Pair<K, V>>> array_it_) : m_table(table), list_it(list_it_), array_it(array_it_) {};

    Pair<K, V>& operator*() {
        if (list_it == (*array_it).end()) {
            throw std::runtime_error("\n[Hash_Iterator] operator *: index out of range\n");
        }
        return *list_it;
    }

    Hash_Iterator& operator++() {
        if (list_it == (*array_it).end()) {
            throw std::runtime_error("\n[Hash_Iterator] operator ++: index out of range\n");
        }
        if (list_it != (*array_it).last()) {
            ++list_it;
        } else {
            do {
                ++array_it;
                if (array_it == m_table->get_array()->end())
                    break;
            } while ((*array_it).empty());
            if (array_it != m_table->get_array()->end()) {
                list_it = (*array_it).begin();
            } else {
                array_it = m_table->get_array()->last();
                list_it = (*array_it).end();
            }
        }
        return *this;
    }

    friend bool operator!= (const Hash_Iterator &it1, const Hash_Iterator &it2) {
        bool a(it1.array_it == it2.array_it);
        bool b(it1.list_it == it2.list_it);
        return !(a & b);
    }

    friend bool operator== (const Hash_Iterator &it1, const Hash_Iterator &it2) {
        bool a(it1.array_it == it2.array_it);
        bool b(it1.list_it == it2.list_it);
        return (a & b);
    }
};

template <typename K, typename V, typename F>
Hash_Iterator<K, V, F> HashTable<K, V, F>::begin() {
    auto array_it = table.begin();
    for (auto &a: table) {
        if (!a.empty())
            return Hash_Iterator<K, V, F>(this, a.begin(), array_it);
        ++array_it;
    }
    return Hash_Iterator<K, V, F>(this, (*(this->table.last())).end(), this->table.last());
}

template <typename K, typename V, typename F>
Hash_Iterator<K, V, F> HashTable<K, V, F>::end() {
    return Hash_Iterator<K, V, F>(this, (*(this->table.last())).end(), this->table.last());
}

template <typename K, typename V, typename F>
bool HashTable<K, V, F>::insert(const K& key, const V& value) {
    bool found(false);
    auto i = hash_func(key);
    for (auto &a: table[i]) {
        if (a.get_first() == key) {
            a.set_second(value);
            found = true;
            break;
        }
    }
    if (!found) {
        table[i].push_back(std::move(Pair<K, V>(key, value)));
        ++elem_number;
        load_factor = (double) elem_number / table.size();
    }
    if (load_factor > rehash_factor_1) rehash_table(q_factor);
    return found;
}

template <typename K, typename V, typename F>
bool HashTable<K, V, F>::remove(const K& key) {
    bool found(false);
    auto i = hash_func(key);
    auto it = table[i].begin();
    for (auto &a: table[i]) {
        if (a.get_first() == key) {
            table[i].erase(it);
            found = true;
            break;
        }
        ++it;
    }
    if (found) {
        --elem_number;
        load_factor = (double) elem_number / table.size();
    }
    if (load_factor < rehash_factor_2) rehash_table(1.0 / q_factor);
    return found;
}

template <typename K, typename V, typename F>
bool HashTable<K, V, F>::contain(const K& key) {
    bool found(false);
    auto i = hash_func(key);
    for (auto &a: table[i]) {
        if (a.get_first() == key) {
            found = true;
            break;
        }
    }
    return found;
}

template <typename K, typename V, typename F>
bool HashTable<K, V, F>::get(const K& key, V& value) {
    bool found(false);
    auto i = hash_func(key);
    for (auto &a: table[i]) {
        if (a.get_first() == key) {
            found = true;
            value = a.get_second();
        }
    }
    return found;
}

template <typename K, typename V, typename F>
Hash_Iterator<K, V, F> HashTable<K, V, F>::get_(const K& key) {
    auto i = hash_func(key);
    for (auto a = table[i].begin(); a != table[i].end(); ++a) {
        if ((*a).get_first() == key) {
            return Hash_Iterator<K, V, F>(this, a, table.make_iterator(i));
        }
    }
    return this->end();
}

template <typename K, typename V, typename F>
void HashTable<K, V, F>::rehash_table(double k) {
    table_size *= k;
    ArraySequence<ListSequence<Pair<K, V>>> new_table(table_size, ListSequence<Pair<K, V>>());
    for (auto &l: table) {
        for (auto &a: l) {
            auto i = hash_func(a.get_first());
            new_table[i].push_back(a);
        }
    }
    table = new_table;
    new_table.clear();
}

template <typename K, typename V, typename F>
HashTable<K, V, F>::~HashTable<K, V, F>() {
    table.clear();
}

template <typename K, typename V, typename F>
unsigned int HashTable<K, V, F>::get_max_bucket_height() {
    unsigned int res(0);
    for (auto &l:table) {
        if (l.size() > res)
            res = l.size();
    }
    return res;
}

template <typename K, typename V, typename F>
void HashTable<K, V, F>::print() {
    for (auto &l: table) {
        if (!l.empty()) l.print();
        else std::cout <<"xxx\n";
    }
}