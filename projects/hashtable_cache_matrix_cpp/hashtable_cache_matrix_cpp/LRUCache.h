#pragma once

#include "Sequences.h"
#include "HashTable.h"

template <typename K, typename V, typename F = HashFunctions<K>>
class LRUCache {
private:
    HashTable<K, V, F> cache;
    HashTable<unsigned int, List_Iterator<unsigned int>> key_to_list;
    ListSequence<unsigned int> lru_queue;
    unsigned int size;

public:
    explicit LRUCache(unsigned int _size) : cache(2 * _size), key_to_list(2 * _size), lru_queue(), size(_size) {}

    Hash_Iterator<K, V, F> get(const K& key); // вовзращает итератор. если ключ не найден, вернется past-the-end итератор
    void push(const K& key, const V& value); // кладет значение в кэш

    Hash_Iterator<K, V, F> end() { return cache.end(); }

    void print() { std::cout << "Cache:\n"; cache.print(); std::cout << "\nLRU_queue:\n"; lru_queue.print(); }
    ListSequence<unsigned int>* get_lru_queue() { return &lru_queue; } //для тестов
    unsigned int get_max_bucket_height() { return cache.get_max_bucket_height(); }
    unsigned int get_size() { return cache.get_size(); }
    unsigned int get_elem_number() { return cache.get_elem_number(); }
};

template<typename K, typename V, typename F>
Hash_Iterator<K, V, F> LRUCache<K, V, F>::get(const K& key) {
    if (cache.get_(key) != cache.end()) { // если ключ есть
        auto list_it = (*(key_to_list.get_(key))).get_second(); // находим ключ в таблице key_to_list, получаем итератор, который указывает на позицию ключа в очереди
        if (list_it != lru_queue.begin()) {
            lru_queue.erase(list_it); // надо переместить ключ в очереди
            lru_queue.push_front(key);
            key_to_list.insert(key, lru_queue.begin()); // обновление таблицы key_to_list
        }
    }
    return cache.get_(key);
}

template<typename K, typename V, typename F>
void LRUCache<K, V, F>::push(const K& key, const V& value) {
    if (cache.get_(key) != cache.end()) { //если ключ уже есть в кэше, надо обновить информацию о последнем вызове
        auto list_it = (*(key_to_list.get_(key))).get_second(); // находим ключ в таблице key_to_list, получаем итератор, который указывает на позицию ключа в очереди
        if (list_it == lru_queue.begin()) { // если ключ и так на первом месте, очередь трогать не надо
            cache.insert(key, value); // надо только обновить значение по ключу в кэше
            return;
        }
        lru_queue.erase(list_it); //удаляем ключ из своего места в очереди по итератору list_it

    } else {
        if (lru_queue.size() == size) { //если кэш заполнен
            unsigned int last_used_key = *(lru_queue.last());
            lru_queue.pop_back(); //убираем из очереди последний элемент
            cache.remove(last_used_key); //убираем этот элемент из кэша
            key_to_list.remove(last_used_key); //убираем его из таблицы key_to_list
        }
    }
    lru_queue.push_front(key); //ставим ключ в начало очереди
    key_to_list.insert(key, lru_queue.begin()); //обновляем информацию об этом ключе в таблице key_to_list
    cache.insert(key, value); //вставляем ключ в кэш (обновляем значение, если ключ уже был в кэше)
}