#pragma once

template <typename T>
class CSRMatrix {
private:
    ArraySequence<T> values; //массив значений
    ArraySequence<int> col_indexes; //массив индексов столбцов
    ArraySequence<int> row_indexes; //массив индексации строк

    int rows; //число строк
    int cols; //число столбцов
    int capacity; //число ненулевых значений

public:
    CSRMatrix(int n, int m, ArraySequence<T>& _values, ArraySequence<int>& _col_indexes, ArraySequence<int>& _row_indexes) :
            rows(n), cols(m), values(_values), col_indexes(_col_indexes), row_indexes(_row_indexes), capacity(_values.size()) {}

    int amount_in_row(int i);

    int get_value(int i);
    int get_row_index(int i);
    int get_col(int i);
    int get_capacity() { return capacity; }

    int get_rows_amount() { return rows; }

    void change(int i, T& v);

    void print();
};

template<typename T>
int CSRMatrix<T>::amount_in_row(int i) {
    if (i >= (row_indexes.size() - 1))
        throw std::runtime_error("\n[CSRMatrix] amount_in_row: index out of range\n");
    return row_indexes[i+1] - row_indexes[i];
}

template<typename T>
int CSRMatrix<T>::get_value(int i) {
    if (i >= values.size())
        throw std::runtime_error("\n[CSRMatrix] get_value: index out of range\n");
    return values[i];
}

template<typename T>
int CSRMatrix<T>::get_col(int i) {
    if (i >= col_indexes.size())
        throw std::runtime_error("\n[CSRMatrix] get_col: index out of range\n");
    return col_indexes[i];
}

template<typename T>
int CSRMatrix<T>::get_row_index(int i) {
    if (i >= row_indexes.size())
        throw std::runtime_error("\n[CSRMatrix] get_row_index: index out of range\n");
    return row_indexes[i];
}

template<typename T>
void CSRMatrix<T>::change(int i, T& v) {
    values[i] = v;
}

template<typename T>
void CSRMatrix<T>::print() {
    int counter(0);
    for (int i = 1; i <= rows; ++i) {
        int max = get_row_index(i) - get_row_index(i-1);
        int j = 0;
        int current_col(0);
        while ((j < max) || (current_col < cols)) {
            if (j < max) {
                if (get_col(counter) == current_col) {
                    std::cout << get_value(counter) << " ";
                    ++j;
                    ++counter;
                } else {
                    std::cout << T() << " ";
                }
            } else {
                std::cout << T() << " ";
            }
            ++current_col;
        }
        std::cout << "\n";
    }
}