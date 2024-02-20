#include <iostream>
#include <random>
#include <fstream>
#include <chrono>
#include <string>

#include <vector>

#include "Graph.h"

class Timer {
private:
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;
    std::chrono::time_point<clock_t> m_beg;
public:
    Timer() : m_beg(clock_t::now()) {}
    void reset() { m_beg = clock_t::now(); }
    [[nodiscard]] double elapsed() const { return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count(); }
};

void small_tests() {
    std::ifstream fin;
    std::string path = R"(C:\Users\Ivan\programming\labs_3sem\lab3_3sem\test_.txt)";
    fin.open(path);
    if (!fin.is_open())
        throw std::runtime_error("\ntest_.txt: file wasn't opened\n");
    int n, m;
    fin >> n >> m;
    std::string tmp; getline(fin, tmp);
    ArraySequence<int> values(m);
    ArraySequence<int> cols(m);
    ArraySequence<int> rows(n+1);
    int q; fin >> q; getline(fin, tmp);
    if (q == 0) {
        for (int i = 0; i < m; ++i) {
            int x, y, z;
            fin >> x >> y >> z;
            --x;
            --y;
            values[i] = z;
            cols[i] = y;
            for (int j = x + 1; j < n + 1; ++j) {
                ++rows[j];
            }
        }
    } else if (q == 1) {
        int counter(0);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int z;
                fin >> z;
                if (z != 0) {
                    values[counter] = z;
                    cols[counter] = j;
                    ++counter;
                }
            }
            rows[i+1] = counter;
        }
    }
    CSRMatrix<int> a(n, n,values, cols, rows);
    Sparse::Graph<int> g(a);
    std::cout << "GRAPH: (adjacency matrix)\n";
    g.print();
    std::cout << "\n\nDIJKSTRA:\n";
    std::cout << "Distances:\n";
    for (int f = 0; f < g.vertex_amount(); ++f) {
        ArraySequence<int> distances(g.vertex_amount(), std::numeric_limits<int>::max());
        ArraySequence<int> parents(g.vertex_amount(), -1);
        g.Dijkstra_algorithm(f, distances, parents);
        for (int i = 0; i < g.vertex_amount(); ++i) {
            if (distances[i] == std::numeric_limits<int>::max()) std::cout << "inf ";
            else std::cout << distances[i] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\nWays:\n";
    for (int j = 0; j < g.vertex_amount(); ++j) {
        ArraySequence<int> distances_(g.vertex_amount(), std::numeric_limits<int>::max());
        ArraySequence<int> parents_(g.vertex_amount(), -1);
        g.Dijkstra_algorithm(j, distances_, parents_);
        for (int i = 0; i < g.vertex_amount(); ++i) {
            std::cout << "from " << j+1 << " to " << i+1 << ": ";
            if (distances_[i] == std::numeric_limits<int>::max()) std::cout << "no way\n";
            else {
                int k = i;
                ListSequence<int> l;
                while (k != -1) {
                    l.push_front(k + 1);
                    k = parents_[k];
                }
                l.print();
            }
        }
    }
    std::cout << "\n\nFLOYD_WARSHALL:\n";
    Matrix<int> _distances(g.vertex_amount(), g.vertex_amount(), std::numeric_limits<int>::max());
    Matrix<int> children(g.vertex_amount(), g.vertex_amount(), std::numeric_limits<int>::max());
    g.Floyd_Warshall_algorithm(_distances, children, std::numeric_limits<int>::max());
    std::cout << "Distances:\n";
    for (int i = 0; i < _distances.rows(); ++i) {
        for (int j = 0; j < _distances.rows(); ++j) {
            if (_distances(i, j) == std::numeric_limits<int>::max()) std::cout << "inf ";
            else std::cout << _distances(i, j) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\nWays:\n";
    //children.print(); std::cout << "\n";
    for (int i = 0; i < children.rows(); ++i) {
        for (int j = 0; j < children.rows(); ++j) {
            std::cout << "from " << i+1 << " to " << j+1 << ": ";
            if (_distances(i, j) == std::numeric_limits<int>::max()) {
                std::cout << "no way\n";
                continue;
            }
            int _i = i;
            while (_i != j) {
                std::cout << _i+1 << " ";
                _i = children(_i, j);
            }
            std::cout << j+1 << "\n";
        }
    }
}

void time_test(int from, int to, int step, int max_weight) {
    //std::string Path = R"(C:\Users\Ivan\programming\labs_3sem\lab3_3sem\TimeTest_Floyd-Warshall.csv)";
    std::string Path = R"(C:\Users\Ivan\programming\labs_3sem\lab3_3sem\TimeTest_2.csv)";
    std::ofstream file(Path);
    file << "Vertex amount, Edge amount, Time(Floyd-Warshall)\n";
    for (int n = from; n <= to; n+=step) {
        std::cout << n << "\n";
        ArraySequence<int> values;
        values.reserve(1020 * n);
        ArraySequence<int> cols;
        cols.reserve(1020 * n);
        ArraySequence<int> rows(n + 1);
        std::default_random_engine gen(n);
        std::uniform_int_distribution<int> probability(1, n);
        std::uniform_int_distribution<int> weights(1, max_weight);
        int counter(0);
        rows[0] = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if ((probability(gen) <= 1000) && (i != j)) {
                    values.push_back(weights(gen));
                    cols.push_back(j);
                    ++counter;
                }
            }
            rows[i + 1] = counter;
            //if (i % 100 == 0)
            //    std::cout << "generate " << i << "\n";
        }
        values.shrink_to_fit();
        cols.shrink_to_fit();
        CSRMatrix<int> a(n, n, values, cols, rows);
        Sparse::Graph<int> g(a);
        double T;
/*
        double T_aver(0);
        int count(0);
        for (int i = 0; i < n; i += (n/20)) {
            //std::cout << "Dijkstra " << i << "\n";
            ++count;
            ArraySequence<int> distances(g.vertex_amount(), std::numeric_limits<int>::max());
            ArraySequence<int> parents(g.vertex_amount(), -1);
            Timer t1;
            g.Dijkstra_algorithm(i, distances, parents);
            T = t1.elapsed();
            T_aver += T;
        }
        T = T_aver / count;
        file << n << "," << g.edge_amount() << "," << T << "\n";
*/

        Matrix<int> _distances(g.vertex_amount(), g.vertex_amount(), std::numeric_limits<int>::max());
        Matrix<int> children(g.vertex_amount(), g.vertex_amount(), std::numeric_limits<int>::max());
        std::cout << "start\n";
        Timer t2;
        g.Floyd_Warshall_algorithm(_distances, children, std::numeric_limits<int>::max());
        T = t2.elapsed();
        std::cout << "end\n";
        //std::cout << n << "," << g.edge_amount() << "," << T << "\n";
        file << n << "," << g.edge_amount() << "," << T << "\n";

    }
}

int main() {
    try {
        small_tests();
        //time_test(4500, 10000, 500, 10000);
    } catch (const std::runtime_error &msg) {
        std::cerr << msg.what();
    }
}