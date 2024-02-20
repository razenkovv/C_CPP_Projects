#pragma once

#include "Vector.h"
#include <memory>
#include <iomanip>

template <typename T>
class Matrix {
private:
    unsigned int m_rows; //число строк
    unsigned int m_cols; //число столбцов
    Vector<T> m_data; //массив с элементами матрицы
public:
    Matrix(); //создание матрицы по умолчанию

    Matrix(unsigned int rows, unsigned int cols, const T &value=T()); //создание матрицы заполненной значением

    Matrix(unsigned int rows, unsigned int cols, T* items); //создание матрицы заполненной значениями из массива

    Matrix(const Matrix<T> &other_matrix); //конструктор копирования

    ~Matrix(); //деструктор

    std::unique_ptr<Matrix<T>> multiply_scalar(const T &value); //домножение матрицы на скаляр

    std::unique_ptr<Matrix<T>> add(Matrix<T> &other_matrix); //сложение двух матриц

    std::unique_ptr<Matrix<T>> multiply(Matrix<T> &other_matrix); //умножение двух матриц

    void add_str(int first, int second, int param=1);
    //добавляет к строке first строку second, если param=1; если param=0, аналогично для столбцов

    void multiply_str(int str, const T &value, int param=1);
    //домножает строку first на число value если param=1; если param=0, аналогично для столбцов

    T norm(); //вычисление нормы матрицы

    void print(); //вывод матрицы на экран

    int rows() { return m_rows; }

    int columns() { return m_cols; }

    T& get(int row, int col); //возвращает значение

    void set(int row, int col, const T &value); //установить значение по индексам

    bool is_equal(Matrix<T> &other_matrix);

    T& operator() (int row, int col); //перегрузка () для возврата значения

    void clear(); //очищает матрицу

    Matrix<T>& operator= (const Matrix<T> &other_matrix); //перегрузка оператора =
};

template <typename T>
Matrix<T>::Matrix() : m_rows{0}, m_cols{0}, m_data() {}

template <typename T>
 Matrix<T>::Matrix(unsigned int rows, unsigned int cols, const T &value) :
    m_rows(rows), m_cols(cols), m_data(rows * cols, value) {}

template <typename T>
Matrix<T>::Matrix(unsigned int rows, unsigned int cols, T* items) :
    m_rows(rows), m_cols(cols), m_data(rows * cols, items) {}

template<typename T>
Matrix<T>::Matrix(const Matrix<T> &other_matrix) :
    m_rows(other_matrix.m_rows), m_cols(other_matrix.m_cols),
    m_data(other_matrix.m_data) {}

template <typename T>
Matrix<T>::~Matrix() {
    clear();
}

template <typename T>
void Matrix<T>::print() {
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            //std::cout << std::setw(5) << std::left << this->get(i , j) << " ";
            std::cout << this->get(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

template <typename T>
T& Matrix<T>::get(int row, int col) {
    if ((row >= m_rows) || (col >= m_cols) || (row < 0) || (col < 0))
        throw std::runtime_error("\n(Matrix) Get Message: Index Out Of Range\n");
    return m_data[m_cols * row + col];
}

template <typename T>
void Matrix<T>::set(int row, int col, const T &value) {
    if ((row >= m_rows) || (col >= m_cols) || (row < 0) || (col < 0))
        throw std::runtime_error("\n(Matrix) Set Message: Index Out Of Range\n");
    m_data[m_cols * row + col] = value;
}

template<typename T>
std::unique_ptr<Matrix<T>> Matrix<T>::multiply_scalar(const T &value) {
    std::unique_ptr<Matrix<T>> res(new Matrix<T>{*this});
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            res->get(i, j) *= value;
        }
    }
    return res;
}

template<typename T>
std::unique_ptr<Matrix<T>> Matrix<T>::add(Matrix<T> &other_matrix) {
    if ((other_matrix.m_rows != m_rows) || (other_matrix.m_cols != m_cols))
        throw std::runtime_error("\n(Matrix) Add Message: Matrices can't be added\n");
    std::unique_ptr<Matrix<T>> res(new Matrix<T>{other_matrix});
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            res->get(i, j) += this->get(i, j);
        }
    }
    return res;
}

template<typename T>
std::unique_ptr<Matrix<T>> Matrix<T>::multiply(Matrix<T> &other_matrix) {
    if (other_matrix.m_rows != m_cols)
        throw std::runtime_error("\n(Matrix) Multiply Message: Matrices can't be multiplied\n");
    std::unique_ptr<Matrix<T>> res(new Matrix<T>{m_rows, other_matrix.m_cols});
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < other_matrix.m_cols; ++j) {
            for (int k = 0; k < m_cols; ++k) {
                res->get(i, j) += this->get(i, k) * other_matrix.get(k, j);
            }
        }
    }
    return res;
}

template<typename T>
void Matrix<T>::add_str(int first, int second, int param) {
    if (param == 1) {
        if ((first >= m_rows) || (second >= m_rows) || (first < 0) || (second < 0))
            throw std::runtime_error("\n(Matrix) Add_str Message: Index Out Of Range\n");
        for (int i = 0; i < m_cols; ++i) {
            this->get(first, i) += this->get(second, i);
        }
    } else if (param == 0) {
        if ((first >= m_cols) || (second >= m_cols) || (first < 0) || (second < 0))
            throw std::runtime_error("\n(Matrix) Add_str Message: Index Out Of Range\n");
        for (int i = 0; i < m_rows; ++i) {
            this->get(i, first) += this->get(i, second);
        }
    } else {
        throw std::runtime_error("\n(Matrix) Add_str Message: Param is unknown (1 for rows, 0 for columns)\n");
    }
}

template<typename T>
void Matrix<T>::multiply_str(int str, const T &value, int param) {
    if (param == 1) {
        if ((str >= m_rows) || (str < 0))
            throw std::runtime_error("\n(Matrix) Multiply_str Message: Index Out Of Range\n");
        for (int i = 0; i < m_cols; ++i) {
            this->get(str, i) *= value;
        }
    } else if (param == 0) {
        if ((str >= m_cols) || (str < 0))
            throw std::runtime_error("\n(Matrix) Multiply_str Message: Index Out Of Range\n");
        for (int i = 0; i < m_rows; ++i) {
            this->get(i, str) *= value;
        }
    } else {
        throw std::runtime_error("\n(Matrix) Multiply_str Message: Param is unknown (1 for rows, 0 for columns)\n");
    }
}

template<typename T>
T Matrix<T>::norm() {
    Vector<T> array{m_rows, T()};
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            array[i] += std::abs(this->get(i, j));
        }
    }
    T max{};
    for (int i = 0; i < m_rows; ++i) {
        if (max < array[i]) max = array[i];
    }
    return max;
}

template<typename T>
void Matrix<T>::clear() {
    m_data.clear();
    m_rows = 0;
    m_cols = 0;
}

template<typename T>
Matrix<T>& Matrix<T>::operator= (const Matrix<T> &other_matrix) {
    if ((other_matrix.m_rows != m_rows) || (other_matrix.m_cols != m_cols))
        throw std::runtime_error("\n(Matrix) operator= Message: Matrices can't be equated\n");
    if (this == &other_matrix)
        return *this;
    m_rows = other_matrix.m_rows;
    m_cols = other_matrix.m_cols;
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            this->get(i, j) = other_matrix.get(i, j);
        }
    }
    return *this;
}

template <typename T>
T& Matrix<T>::operator() (int row, int col) {
    if ((row >= m_rows) || (col >= m_cols) || (row < 0) || (col < 0))
        throw std::runtime_error("\n(Matrix) Operator() Message: Index Out Of Range\n");
    return m_data[m_cols * row + col];
}

template<typename T>
bool Matrix<T>::is_equal(Matrix<T> &other_matrix) {
    if ((m_rows != other_matrix.m_rows) || (m_cols != other_matrix.m_cols))
        return false;
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            if (this->get(i, j) != other_matrix.get(i, j)) {
                return false;
            }
        }
    }
    return true;
}

