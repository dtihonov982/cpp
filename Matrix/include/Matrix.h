#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cstring>
#include <cassert>
#include <iterator>

template<typename T>
class Matrix {
public:
    Matrix();
    Matrix(int rows_, int cols_);
    Matrix(int rows_, int cols_, const T& value);
    Matrix(std::initializer_list<std::initializer_list<T>> lst); 

    Matrix(const Matrix<T>& other);
    Matrix<T>& operator=(const Matrix<T>& other);
    Matrix(Matrix<T>&& other) noexcept;
    Matrix<T>& operator=(Matrix<T>&& other) noexcept;
    ~Matrix();
    
    void fill(const T& value);

    struct RowProxy {
        T* row;
        const T& operator[](int i) const { return row[i]; }
        T& operator[](int i) { return row[i]; }
    };   
    RowProxy operator[](int i) { return RowProxy{data + i * cols}; }
    const RowProxy operator[](int i) const { return RowProxy{data + i * cols}; }
    T& at(int i, int j) { return data[i * cols + j]; }
    const T& at(int i, int j) const { return data[i * cols + j]; }

    void dump(std::ostream& out) const;
    Matrix clone() const;
    
    void product(const T& x); 
    Matrix<T> product(const Matrix<T>& B) const;
    Matrix<T> pow(int n) const;
    Matrix<T> minorMatrix(int exRow, int exCol) const;
    T det() const;
    
    static bool same_shapes(const Matrix& lhs, const Matrix& rhs);
    static Matrix loadFromDump(std::istream& is);
    
private:
    void allocate();
    void free();
    void copy(const Matrix<T>& rhs);
    
    T* data = nullptr;
    int rows = 0;
    int cols = 0;
#ifndef NDEBUG
    int id_ = 0;
    static int getID() {
        static int lastID = 0;
        return ++lastID;
    }
    void setID() { 
        if (!id_)
            id_ = getID(); 
    }
    void log(std::string_view sv) {
        std::cout << "#" << id_ << "\t";
        std::cout <<"this=" << this << "\tdata=" << data;    
        std::cout << "\t" << sv << '\n';    
    }  
#endif
};

template<typename T>
Matrix<T> Matrix<T>::clone() const {
    return Matrix{*this}; 
}

template<typename T>
void Matrix<T>::allocate() {
    data = new T[rows * cols];
}

template<typename T>
void Matrix<T>::free() {
    delete[] data;
}

template<typename T>
void Matrix<T>::fill(const T& value) {
    std::fill(data, data + rows * cols, value);
}

template<typename T>
Matrix<T>::Matrix() {
#ifndef NDEBUG
    setID();
    log("default ctor");
#endif
}

template<typename T>
Matrix<T>::Matrix(int rows_, int cols_) {
    rows = rows_;
    cols = cols_;
    allocate();
#ifndef NDEBUG
    setID();
    log("(n, m) ctor");
#endif
}

template<typename T>
Matrix<T>::Matrix(int rows_, int cols_, const T& value)
: Matrix(rows_, cols_) {
    fill(value);
#ifndef NDEBUG
    setID();
    log("(n, m, value) ctor");
#endif
}

template<typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> lst) {
    rows = lst.size();
    cols = lst.begin()->size();
    allocate();
    
    T* ptr = data;
    for (const auto& row: lst) {
        std::copy(row.begin(), row.end(), ptr);
        ptr += cols;
    }
#ifndef NDEBUG
    setID();
    log("init list ctor");
#endif
}

template<typename T>
void Matrix<T>::copy(const Matrix<T>& other) {
    rows = other.rows;
    cols = other.cols;
    allocate();
    std::copy(other.data, other.data + other.rows * other.cols, data);
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& other) {
    copy(other);
#ifndef NDEBUG
    setID();
    log("copy ctor");
#endif
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
    if (this == &other) {
        return *this;
    }
    free();
    copy(other);
#ifndef NDEBUG
    log("copy assignment");
#endif
    return *this;
}

template<typename T>
Matrix<T>::Matrix(Matrix<T>&& other) noexcept {
    data = other.data;
    rows = other.rows;
    cols = other.cols;
    other.data = nullptr;
#ifndef NDEBUG
    setID();
    log("move ctor");
#endif
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    free();
    data = other.data;
    rows = other.rows;
    cols = other.cols;
    other.data = nullptr;
#ifndef NDEBUG
    log("move assignment");
#endif
    return *this;
}

template<typename T>
Matrix<T>::~Matrix() {
#ifndef NDEBUG
    log("dtor");
#endif
    free();    
}

template<typename T>
void Matrix<T>::dump(std::ostream& out) const {
    out << rows << ' ' << cols << std::endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            out << at(i, j) << " ";
        }
        out << std::endl;
    }
}

template<typename T>
Matrix<T> Matrix<T>::product(const Matrix& B) const {
    if (cols != B.rows) {
        throw std::runtime_error("Shapes of matrixes are incorrect for multiplication.");
    }
    Matrix<T> result(rows, B.cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < B.cols; ++j) {
            T sum{};
            for (int k = 0; k < cols; ++k) {
                sum += at(i, k) * B.at(k, j);

            }
            result.at(i, j) = sum;
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
void Matrix<T>::product(const T& x) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            at(i, j) *= x;
        }
    }
}

#if 0
template<typename T>
Matrix<T> operator*(const T& number, const Matrix<T>& matrix) {
    matrix.product(number);
    return matrix;
}

template<typename T>
Matrix<T> operator*(const Matrix<T>& matrix, const T& number) {
    matrix.product(number);
    return matrix;
}
#endif

template<typename T>
Matrix<T> Matrix<T>::minorMatrix(int exRow, int exCol) const {
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
T Matrix<T>::det() const {
    if (rows != cols) {
        throw "Matrix is not square matrix.";
    }
    if (rows == 1 && cols == 1) {
        return operator[](0)[0];
    }
    T result{};
    for (int j = 0, sign = -1; j < cols; ++j) {
        Matrix<T> minor = minorMatrix(0, j);
        sign *= -1;
        result += sign * operator[](0)[j] * minor.det();
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::loadFromDump(std::istream& is) {
    std::istream_iterator<T> it{is}, end;
    int rows_ = *it++;
    int cols_ = *it++;
    Matrix result(rows_, cols_);
    std::copy(it, end, result.data);    
    return result;
}

#endif
