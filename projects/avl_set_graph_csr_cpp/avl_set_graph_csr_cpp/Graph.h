#pragma once

#include "Sequences.h"
#include "Pair.h"
#include "CSRMatrix.h"
#include "Matrix.h"
#include "Set.h"

namespace Sparse {
    template<typename T>
    class Graph {
    protected:
        CSRMatrix<T> matrix; //CSR матрица смежности

    public:
        explicit Graph(CSRMatrix<T> &matrix_) : matrix(matrix_) {};

        void Dijkstra_algorithm(int x, ArraySequence<T> &distances, ArraySequence<int> &parents);

        void Floyd_Warshall_algorithm(Matrix<T> &distances, Matrix<T> &children, T max);

        int vertex_amount() { return matrix.get_rows_amount(); }

        int edge_amount() { return matrix.get_capacity(); }

        void print();
    };

    template<typename T>
    void Graph<T>::Dijkstra_algorithm(int x, ArraySequence<T> &distances, ArraySequence<int> &parents) {
        distances.set(x, T());
        Set<Pair<T, int>> s;
        s.insert(Pair<T, int>(distances[x], x));
        while (!s.empty()) {
            auto p = s.minimum();
            auto i = p.get_second();
            s.remove(p);
            for (int counter = matrix.get_row_index(i); counter < matrix.get_row_index(i + 1); ++counter) {
                auto weight = matrix.get_value(counter);
                auto j = matrix.get_col(counter);
                if ((distances[i] + weight) < distances[j]) {
                    s.remove(Pair<T, int>(distances[j], j));
                    distances[j] = distances[i] + weight;
                    parents[j] = i;
                    s.insert(Pair<T, int>(distances[j], j));
                }
            }
        }
    }

    template<typename T>
    void Graph<T>::Floyd_Warshall_algorithm(Matrix<T> &distances, Matrix<T> &children, T max) {
        int counter(0);
        for (int i = 0; i < matrix.get_rows_amount(); ++i) {
            for (int j = 0; j < (matrix.get_row_index(i+1) - matrix.get_row_index(i)); ++j) {
                distances(i, matrix.get_col(counter)) = matrix.get_value(counter);
                children(i, matrix.get_col(counter)) = matrix.get_col(counter);
                ++counter;
            }
            children(i, i) = i;
            distances(i, i) = 0;
        }
        for (int k = 0; k < distances.rows(); ++k) {
            for (int i = 0; i < distances.rows(); ++i) {
                for (int j = 0; j < distances.rows(); ++j) {
                    if ((distances(i, k) < max) && (distances(k, j) < max)) {
                        if (distances(i, j) > (distances(i, k) + distances(k, j))) {
                            distances(i, j) = distances(i, k) + distances(k, j);
                            children(i, j) = children(i, k);
                        }
                    }
                }
            }
        }
    }

    template<typename T>
    void Graph<T>::print() {
        matrix.print();
    }
}