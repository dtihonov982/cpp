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
	
	Matrix(const Matrix<T>& other);
	Matrix<T>& operator=(const Matrix<T>& other);
	Matrix(Matrix<T>&& other) noexcept;
	Matrix<T>& operator=(Matrix<T>&& other) noexcept;
	~Matrix();
	
	void output(std::ostream& out) const;
	void loadFile(std::string filename);
	
	void free();
	void copy(const Matrix<T>& rhs);

	T* operator[](int i);

	T& at(int i, int j);
	Matrix<T> product(const Matrix& B) const;
	Matrix<T> pow(int n) const;
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
Matrix<T>::Matrix(int n_, int m_) {
    std::cout << "# Matrix(n, m) ctor. " << this << std::endl;
    data = new T*[m_];
    for (int i = 0; i < m_; ++i) {
        data[i] = new T[n_];
    }
    rows = m_;
    cols = n_;
}

template<typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> lst) {
    std::cout << "# Matrix init_lst ctor. " << this << std::endl;
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
void Matrix<T>::loadFile(std::string filename) {
    

}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& other) {
#ifdef DEBUG
    std::cout << "# Matrix copy ctor. " << this << std::endl;
#endif
    std::memcpy(data, other.data, other.rows * other.cols * sizeof(T));
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
#ifdef DEBUG
    std::cout << "# Matrix copy assignment. " << this << std::endl;
#endif
    if (this == &other) {
        return *this;
    }
    free();
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
    free();
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

template<typename T>
void Matrix<T>::copy(const Matrix<T>& other) {
    rows = other.rows;
    cols = other.cols;
    data = new T*[rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new T[cols];
        for (int j = 0; j < cols; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
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
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            out << data[i][j] << " ";
        }
        out << std::endl;
    }
}



template<typename T>
T& Matrix<T>::at(int i, int j) {
    return data[i][j];
}

template<typename T>
T* Matrix<T>::operator[](int i) {
    return data[i];
}


template<typename T>
Matrix<T> Matrix<T>::product(const Matrix& B) const {
    if (cols != B.rows) {
        throw std::invalid_argument();
    }
    Matrix<T> result(rows, B.cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < B.cols; ++j) {
	    //? is ~T invokes in this cycle?
            T sum{};
            for (int k = 0; k < cols; ++k) {
                sum += data[i][k]*B.data[k][j];
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


#endif
