#pragma once

#include "Sequences.h"
#include "Iterator.h"

template <typename T>
void swap(T& x, T& y);

template <typename T>
void insert_sort(Iterator<T>& b, Iterator<T>& e, int (*cmp) (const T&, const T&));

template <typename T>
void quick_sort(Iterator<T>& b, Iterator<T>& e, int (*cmp) (const T&, const T&));

template <typename T>
void merge_sort(Iterator<T>& b, Iterator<T>& e, int (*cmp) (const T&, const T&));
template <typename T>
void merge(Iterator<T>& b, Iterator<T>& e, Iterator<T>& middle, int (*cmp) (const T&, const T&));

template <typename T>
void heap_sort(Iterator<T>& b, Iterator<T>& e, int (*cmp) (const T&, const T&));
template <typename T>
void heapify(Iterator<T>& b, Iterator<T>& e, int (*cmp) (const T&, const T&));

template <typename T>
bool check_sorted(Iterator<T>& b, Iterator<T>& e, int (*cmp) (const T&, const T&));


template<typename T>
void swap(T& x, T& y)
{
    T tmp { std::move(x) };
    x = std::move(y);
    y = std::move(tmp);
}

template <typename T>
void insert_sort(Iterator<T>& b, Iterator<T>& e, int (*cmp) (const T&, const T&)) {
    auto begin = b + 1;
    while (*begin <= e) {
        auto run = *begin - 1;
        auto next_run = *run + 1;
        while ((cmp(**run, **next_run)) > 0) {
            swap(**run, **next_run);
            if (*run != b) {
                --*run;
                --*next_run;
            } else break;
        }
        ++*begin;
    }
}

template <typename T>
void quick_sort(Iterator<T>& b, Iterator<T>& e, int (*cmp) (const T&, const T&)) {
    if ((e - b) <= 1) {
        if ((e - b) <= 0) return;
        else {
            if (cmp(*b, *e) > 0) swap(*b, *e);
            return;
        }
    }
    auto begin = b.copy();
    auto end = e.copy();
    T pivot_value = **(b + (e - b)/2);
    int pivot(b.get_index() + (e - b)/2);
    do {
        while (cmp(*b, pivot_value) < 0)
            ++b;
        while (cmp(*e, pivot_value) > 0)
            --e;
        if (b < e) {
            swap(*b, *e);
            if (pivot == b.get_index())
                pivot = e.get_index();
            else if (pivot == e.get_index())
                pivot = b.get_index();
            ++b;
            --e;
        } else if (b == e) {
            ++b;
            --e;
            break;
        }
    } while (b <= e);
    quick_sort(*begin, e, cmp);
    quick_sort(++e, *end, cmp);
}

template <typename T>
void merge_sort(Iterator<T>& b, Iterator<T>& e, int (*cmp) (const T&, const T&)) {
    if (b >= e) {
        return;
    }
    auto middle = b + (e - b)/2;
    merge_sort(b, *middle, cmp);
    merge_sort(*(*middle + 1), e, cmp);
    merge(b, e, *middle, cmp);
}

template <typename T>
void merge(Iterator<T>& b, Iterator<T>& e, Iterator<T>& middle, int (*cmp) (const T&, const T&)) {
    if ((b >= e) || (middle < b) || (middle > e)) return;
    if (e == *(b + 1)) {
        if (cmp(*b, *e) > 0) swap(*b, *e);
        return;
    }
    std::unique_ptr<ArraySequence<T>> tmp(new ArraySequence<T>{static_cast<unsigned int> (e - b + 1)});
    auto run = b.copy();
    for (int i = 0; i <= (e - b); ++i) {
        new(&(tmp->get(i))) T(**run);
        ++*run;
    }
    int j(0), k(middle - b + 1);
    auto begin = b.copy();
    while (*begin <= e) {
        if (j > (middle - b)) {
            **begin = tmp->get(k);
            ++k;
        } else if (k > (e - b)) {
            **begin = tmp->get(j);
            ++j;
        } else {
            if (cmp(tmp->get(j), tmp->get(k)) < 0) {
                **begin = tmp->get(j);
                ++j;
            } else {
                **begin = tmp->get(k);
                ++k;
            }
        }
        ++*begin;
    }
}

template <typename T>
void heap_sort(Iterator<T>& b, Iterator<T>& e, int (*cmp) (const T&, const T&)) {
    if ((e - b) <= 1) {
        if (cmp(*b, *e) > 0)
            swap(*b, *e);
        return;
    }
    auto i = b + ((e - b + 1) / 2 - 1);
    while (true) { //нельзя написать while (*i >= b) т.к. нельзя применить -- к b (к началу (метод begin) sequence)
        heapify(b, e, *i, cmp);
        if (*i != b) --*i;
        else break;
    }
    while (true) {
        swap(*b, *e);
        if (e != b) {
            --e;
            heapify(b, e, b, cmp);
        } else break;
    }
}

template <typename T>
void heapify(Iterator<T>& b, Iterator<T>& e, Iterator<T>& i, int (*cmp) (const T&, const T&)) {
    bool left_exists(false), right_exists(false);
    std::unique_ptr<Iterator<T>> left(nullptr), right(nullptr);
    if ((2 * (i - b) + 1) <= (e - b)) {
        left_exists = true;
        left = b + (2 * (i - b) + 1);
    }
    if ((2 * (i - b) + 2) <= (e - b)) {
        right_exists = true;
        right = b + (2 * (i - b) + 2);
    }
    std::unique_ptr<Iterator<T>> largest_(nullptr), largest(nullptr);
    if ((left_exists) && (cmp(**left, *i) > 0))
        largest_ = left->copy();
    else
        largest_ = i.copy();
    if ((right_exists) && (cmp(**right, **largest_) > 0))
        largest = right->copy();
    else
        largest = largest_->copy();
    if (*largest != i) {
        swap(**largest, *i);
        heapify(b, e, *largest, cmp);
    }
}

template <typename T>
bool check_sorted(Iterator<T>& b, Iterator<T>& e, int (*cmp) (const T&, const T&)) {
    bool check = true;
    auto c = b.copy();
    ++*c;
    for (b; b != e; ++b, ++*c) {
        if (cmp(*b, **c) > 0) {
            check = false;
            break;
        }
    }
    if (!check)
        std::cout << "Check_sorted: ERROR.\n";
    else
        std::cout << "Check_sorted: OK.\n";
    return check;
}

//template <typename T>
//void quick_sort_list(ListSequence<T>& seq, typename List<T>::Node* start, typename List<T>::Node* end, int (*cmp) (const T&, const T&)) {
//    if (start == end)
//        return;
//    typename List<T>::Node* b = start;
//    typename List<T>::Node* pivot = end;
//    while (b != pivot) {
//        if (cmp(b->value, pivot->value) > 0) {
//            if (b == start)
//                start = b->next;
//            typename List<T>::Node *tmp = b;
//            b = b->next;
//            seq.insert_after(tmp, end);
//            end = tmp;
//        } else {
//            b = b->next;
//        }
//    }
//
//    if (b->prev)
//        quick_sort_list(seq, start, b->prev, cmp);
//    if (b->next)
//        quick_sort_list(seq, b, end, cmp);
//}
//
//template <typename T>
//void quick_sort_array(ArraySequence<T>& seq, int start, int n, int (*cmp) (const T&, const T&)) {
//    if (n <= 1)
//        return;
//    int i{start}, j{start + n - 1};
//    int pivot{start + n / 2};
//    do {
//        while (cmp(seq.get(i), seq.get(pivot)) < 0)
//            ++i;
//        while (cmp(seq.get(j), seq.get(pivot)) > 0)
//            --j;
//        if (i < j) {
//            swap(seq.get(i), seq.get(j));
//            if (pivot == i)
//                pivot = j;
//            else if (pivot == j)
//                pivot = i;
//            ++i;
//            --j;
//        } else if (i == j) {
//            ++i;
//            --j;
//            break;
//        }
//    } while (i <= j);
//
//    int len = j - start + 1;
//    int new_start = start + len;
//    quick_sort_array(seq, start, len, cmp);
//    quick_sort_array(seq, new_start, n - len, cmp);
//}
//template <typename T>
//void insert_sort(Sequence<T>& seq, int start, int n, int (*cmp) (const T&, const T&)) {
//    if ((start + n > seq.size()) || (start < 0) || (n < 0))
//        throw std::runtime_error("\nInsert_sort Message: index out of range\n");
//    for (int i = start; i < start + n; ++i) {
//        int j = i - 1;
//        while ((j >= 0) && (cmp(seq.get(j), seq.get(j+1))) > 0) {
//            swap(seq.get(j), seq.get(j+1));
//            --j;
//        }
//    }
//}