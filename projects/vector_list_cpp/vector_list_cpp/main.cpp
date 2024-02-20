#include <iostream>
#include<fstream>
#include <random>
#include <array>
#include <chrono>
#include <algorithm>

#include "Sequences.h"
#include "Sort.h"

class Timer
{
private:
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;
    std::chrono::time_point<clock_t> m_beg;

public:
    Timer() : m_beg(clock_t::now()) {}

    void reset() {
        m_beg = clock_t::now();
    }

    double elapsed() const {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }
};

int cmp(const int& x,const int& y) {
    if (x > y) return 1;
    else if (x == y) return 0;
    else return -1;
}

int cmp_r(const int& x,const int& y) {
    if (x > y) return -1;
    else if (x == y) return 0;
    else return 1;
}

void test_time() {
    std::cout << "Choose sort:\n1 - merge_sort\n2 - heap_sort\n3 - quick_sort\n4 - insert_sort\n";
    int a;
    std::cin >> a;
    std::cout << "Enter bounds of number of elements in sequences:\n";
    int left, right;
    std::cin >> left >> right;
    std::cout << "Enter step:\n";
    int step;
    std::cin >> step;
    std::cout << "Enter number of iterations:\n";
    int k;
    std::cin >> k;

    switch (a) {
        case 1: {
            std::string Path_merge = R"(C:\Users\Ivan\programming\labs\lab1_3sem\ArrayMergeSort_10^5-10^6.txt)";
            std::ofstream fout_merge(Path_merge);
            for (int i = left; i <= right; i += step) {
                fout_merge << i << ", ";
            }
            fout_merge << "\n";
            for (unsigned int n = left; n <= right; n += step) {
                if (n % 100 == 0)
                    std::cout << n << "\n";
                double T_merge(0);
                for (int i = 0; i < k; ++i) {
                    std::default_random_engine gen(n + i);
                    std::uniform_int_distribution<int> distr(-1000000, 1000000);
                    ArraySequence<int> seq{n, 0};
                    for (int &it: seq) {
                        it = distr(gen);
                    }
                    auto it1 = seq.begin();
                    auto it2 = seq.last();
                    Timer t1;
                    merge_sort(it1, it2, cmp);
                    T_merge += t1.elapsed();
                }
                T_merge = T_merge / k;
                fout_merge << T_merge << ", ";
            }
            break;
        }

        case 2: {
            std::string Path_heap = R"(C:\Users\Ivan\programming\labs\lab1_3sem\ArrayHeapSort_10^5-10^6.txt)";
            std::ofstream fout_heap(Path_heap);
            for (int i = left; i <= right; i += step) {
                fout_heap << i << ", ";
            }
            fout_heap << "\n";
            for (unsigned int n = left; n <= right; n += step) {
                if (n % 100 == 0)
                    std::cout << n << "\n";
                double T_heap(0);
                for (int i = 0; i < k; ++i) {
                    std::default_random_engine gen(n + i);
                    std::uniform_int_distribution<int> distr(-1000000, 1000000);
                    ArraySequence<int> seq{n, 0};
                    for (int &it: seq) {
                        it = distr(gen);
                    }
                    auto it1 = seq.begin();
                    auto it2 = seq.last();
                    Timer t2;
                    heap_sort(it1, it2, cmp);
                    T_heap += t2.elapsed();
                }
                T_heap = T_heap / k;
                fout_heap << T_heap << ", ";
            }
            break;
        }

        case 3: {
            std::string Path_quick = R"(C:\Users\Ivan\programming\labs\lab1_3sem\ArrayQuickSort_10^5-10^6.txt)";
            std::ofstream fout_quick(Path_quick);
            for (int i = left; i <= right; i += step) {
                fout_quick << i << ", ";
            }
            fout_quick << "\n";
            for (unsigned int n = left; n <= right; n += step) {
                if (n % 100 == 0)
                    std::cout << n << "\n";
                double T_quick(0);
                for (int i = 0; i < k; ++i) {
                    std::default_random_engine gen(n + i);
                    std::uniform_int_distribution<int> distr(-1000000, 1000000);
                    ArraySequence<int> seq{n, 0};
                    for (int &it: seq) {
                        it = distr(gen);
                    }
                    auto it1 = seq.begin();
                    auto it2 = seq.last();
                    Timer t3;
                    quick_sort(it1, it2, cmp);
                    T_quick += t3.elapsed();
                }
                T_quick = T_quick / k;
                fout_quick << T_quick << ", ";
            }
            break;
        }

        case 4: {
            std::string Path_insert = R"(C:\Users\Ivan\programming\labs\lab1_3sem\ArrayInsertSort_test.txt)";
            std::ofstream fout_insert(Path_insert);
            for (int i = left; i <= right; i += step) {
                fout_insert << i << ", ";
            }
            fout_insert << "\n";
            for (unsigned int n = left; n <= right; n += step) {
                if (n % 100 == 0)
                    std::cout << n << "\n";
                double T_insert(0);
                for (int i = 0; i < k; ++i) {
                    std::default_random_engine gen(n + i);
                    std::uniform_int_distribution<int> distr(-1000000, 1000000);
                    ArraySequence<int> seq{n, 0};
                    for (int &it: seq) {
                        it = distr(gen);
                    }
                    auto it1 = seq.begin();
                    auto it2 = seq.last();
                    Timer t4;
                    insert_sort(it1, it2, cmp);
                    T_insert += t4.elapsed();
                }
                T_insert = T_insert / k;
                fout_insert << T_insert << ", ";
            }
            break;
        }

        default:
            break;
    }
}

void enter_sequence() {
    std::cout << "Enter amount of elements and then the elements (integers):\n";
    unsigned int n;
    std::cin >> n;
    ArraySequence<int> seq{n , 0};
    for (int &it: seq) {
        int b;
        std::cin >> b;
        it = b;
    }
    auto seq1 = seq.copy(); auto seq2 = seq.copy(); auto seq3 = seq.copy(); auto seq4 = seq.copy();

    auto it1 = seq1->begin();
    auto it1_ = seq1->last();
    merge_sort(it1, it1_, cmp);
    std::cout << "merge_sort:\n";
    seq1->print();
    it1 = seq1->begin();
    it1_ = seq1->last();
    check_sorted(it1, it1_, cmp);
    std::cout << "\n";

    auto it2 = seq2->begin();
    auto it2_ = seq2->last();
    quick_sort(it2, it2_, cmp);
    std::cout << "quick_sort:\n";
    seq2->print();
    it2 = seq2->begin();
    it2_ = seq2->last();
    check_sorted(it2, it2_, cmp);
    std::cout << "\n";

    auto it3 = seq3->begin();
    auto it3_ = seq3->last();
    heap_sort(it3, it3_, cmp);
    std::cout << "heap_sort:\n";
    seq3->print();
    it3 = seq3->begin();
    it3_ = seq3->last();
    check_sorted(it3, it3_, cmp);
    std::cout << "\n";

    auto it4 = seq4->begin();
    auto it4_ = seq4->last();
    insert_sort(it4, it4_, cmp);
    std::cout << "insert_sort:\n";
    seq4->print();
    it4 = seq4->begin();
    it4_ = seq4->last();
    check_sorted(it4, it4_, cmp);
    std::cout << "\n";
}

int main() {
    //test_time();
    enter_sequence();
}