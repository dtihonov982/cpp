#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cstring>

template<typename T>
class Matrix {
public:
    Matrix();
    Matrix(int m_, int n_);
    Matrix(std::initializer_list<std::initializer_list<T>> lst); 
    Matrix(std::istream& is);
    
    class Row {
    private:
        T* row;
    public:
        Row(T* row_): row(row_) {}
        const T& operator[](int i) const { return row[i]; }
        T& operator[](int i) { return row[i]; }
    };

    
    class iterator {
        T* ptr;
    public:
        T& operator++();
        T operator++(int);
    };

    Matrix(const Matrix<T>& other);
    Matrix<T>& operator=(const Matrix<T>& other);
    Matrix(Matrix<T>&& other) noexcept;
    Matrix<T>& operator=(Matrix<T>&& other) noexcept;
    ~Matrix();
    
    void output(std::ostream& out) const;
    
    void free();
    void copy(const Matrix<T>& rhs);

    Row operator[](int i) { return Row(data + i * cols); }
    const Row operator[](int i) const { return Row(data + i * cols); }
    Matrix<T> product(const T& x) const; 

    T& at(int i, int j);
    Matrix<T> product(const Matrix<T>& B) const;
    Matrix<T> pow(int n) const;
    Matrix<T> minorMatrix(int exRow, int exCol);
    T det() const;
private:
    T* data = nullptr;
    int rows = 0;
    int cols = 0;
};

template<typename T>
Matrix<T>::Matrix() {
#ifdef DEBUG
    std::cout << "# Matrix default ctor. " << this << std::endl;
#endif
}

template<typename T>
Matrix<T>::Matrix(int rows_, int cols_) {
#ifdef DEBUG
    std::cout << "# Matrix(n, m) ctor. " << this << std::endl;
#endif
    rows = rows_;
    cols = cols_;
    data = new T[rows * cols];
}

template<typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> lst) {
#ifdef DEBUG
    std::cout << "# Matrix init_lst ctor. " << this << std::endl;
#endif
    rows = lst.size();
    cols = lst.begin()->size();
    data = new T[rows * cols];
    int i = 0;
    for (auto it = lst.begin(), et = lst.end(); it != et; ++it) {
        for (auto jt = it->begin(), jte = it->end(); jt != jte; ++jt) {
           data[i++] = *jt; 
        }
    }
}

template<typename T>
Matrix<T>::Matrix(std::istream& is) {
    is >> rows;
    is >> cols;
    data = new T[rows * cols];
    for (int i = 0; i < rows * cols; ++i) {
        is >> data[i];
    }
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& other) {
#ifdef DEBUG
    std::cout << "# Matrix copy ctor. " << this << std::endl;
#endif
    copy(other);
}

template<typename T>
void Matrix<T>::copy(const Matrix<T>& other) {
    rows = other.rows;
    cols = other.cols;
    data = new T[rows * cols];
    std::copy(other.data, other.data + other.rows * other.cols, data);
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
#ifdef DEBUG
    std::cout << "# Matrix copy assignment. " << this << std::endl;
#endif
    if (this == &other) {
        return *this;
    }
    delete[] data;
    copy(other);
    return *this;
}

template<typename T>
Matrix<T>::Matrix(Matrix<T>&& other) noexcept {
#ifdef DEBUG
    std::cout << "# Matrix move ctor. " << this << std::endl;
#endif
    data = other.data;
    rows = other.rows;
    cols = other.cols;
    other.data = nullptr;
    other.rows = 0;
    other.cols = 0;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) noexcept {
#ifdef DEBUG
    std::cout << "# Matrix move assign. " << this << std::endl;
#endif
    if (this == &other) {
        return *this;
    }
    delete[] data;
    data = other.data;
    rows = other.rows;
    cols = other.cols;
    other.data = nullptr;
    other.rows = 0;
    other.cols = 0;
    return *this;
}

template<typename T>
Matrix<T>::~Matrix() {
#ifdef DEBUG
    std::cout << "# Matrix dtor  " << this << " data=" << data << std::endl;
#endif
    delete[] data;    
}

#if 0
template<typename T>
void Matrix<T>::free() {
    for (int i = 0; i < rows; ++i) {
        delete[] data[i];
    }
    delete[] data;
}
#endif

template<typename T>
void Matrix<T>::output(std::ostream& out) const {
    out << rows << ' ' << cols << std::endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            out << data[cols * i + j] << " ";
        }
        out << std::endl;
    }
}

#if 0 
template<typename T>
T* Matrix<T>::operator[](int i) {
    return data + i * rows;
}
#endif

template<typename T>
Matrix<T> Matrix<T>::product(const Matrix& B) const {
    if (cols != B.rows) {
        throw std::runtime_error("Shapes of matrixes are incorrect for multiplication.");
    }
    Matrix<T> result(rows, B.cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < B.cols; ++j) {
            //? is ~T invokes in this cycle?
            T sum{};
            for (int k = 0; k < cols; ++k) {
                sum += operator[](i)[k] * B[k][j];

            }
            result[i][j] = sum;
        }
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::pow(int n) const {
    Matrix<T> result(*this);
    if (n == 1) {
        return result;
    }
    for (int i = 1; i < n; ++i) {
        result = result.product(*this);
    }
    return result;

}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& M) {
    M.output(os);
    return os;
}

template<typename T>
Matrix<T> Matrix<T>::product(const T& x) const {
    Matrix<T> result{*this};
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] *= x;
        }
    }
    return result;
}

template<typename T>
Matrix<T> operator*(const T& number, const Matrix<T>& matrix) {
    return matrix.product(number);
}

template<typename T>
Matrix<T> operator*(const Matrix<T>& matrix, const T& number) {
    return matrix.product(number);
}

/*
template<typename T>
void fillRand(int start, end int) {
    std::srand(std::time(NULL));
    for (int i = 0; i < 

template<typename T>
T Matrix<T>::det() const {
}
*/

template<typename T>
Matrix<T> Matrix<T>::minorMatrix(int exRow, int exCol) {
    if (exRow > rows || exCol > cols) {
        throw std::runtime_error{"Out of range"}; 
    }
    Matrix<T> result(rows - 1, cols - 1);
    for (int isrc = 0, idst = 0; isrc < rows; ++isrc) {
        if (isrc == exRow) continue;
        for (int jsrc = 0, jdst = 0; jsrc < cols; ++jsrc) {
            if (jsrc == exCol) continue;
            result[idst][jdst] = operator[](isrc)[jsrc];
            ++jdst;
        }
        ++idst;
    }
    return result;
}

template<typename T>
T det() {
    if (rows != cols) {
        throw "Matrix is not square matrix.";
    }
    if (rows == 1 && cols == 1) {
        return operator[](0)[0];
    }
    T result{};
    for (int j = 0, sign = -1; j < cols; ++j) {
        minor = minorMatrix(0, j);
        sign *= -1;
        result += sign * operator[](0)[j] * minor.det();
    }
    return result;
}







#endif
