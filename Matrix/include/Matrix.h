#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

template<typename T>
class Matrix {
public:
	Matrix();
	Matrix(int m_, int n_);
	Matrix(std::initializer_list<std::initializer_list<T>> lst);
	Matrix(const Matrix<T>& rhs);
	~Matrix();
	void output(std::ostream& out);
	void loadFile(std::string filename);
	void free();
	void copy(const Matrix<T>& rhs);

	T* operator[](int i);
	Matrix<T>& operator=(const Matrix<T>& rhs);
	T& at(int i, int j);
	Matrix<T> product(const Matrix& B);
	Matrix<T> pow(int n);
private:

	T** data = nullptr;
	int rows = 0;
	int cols = 0;
};

template<typename T>
Matrix<T>::Matrix() {}

template<typename T>
Matrix<T>::Matrix(int n_, int m_) {
    data = new T*[m_];
    for (int i = 0; i < m_; ++i) {
        data[i] = new T[n_];
    }
    rows = m_;
    cols = n_;
}

template<typename T>
void Matrix<T>::loadFile(std::string filename) {


}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& rhs) {
    copy(rhs);
}

template<typename T>
void Matrix<T>::copy(const Matrix<T>& rhs) {
    rows = rhs.rows;
    cols = rhs.cols;
    data = new T*[rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new T[cols];
        for (int j = 0; j < cols; ++j) {
            data[i][j] = rhs.data[i][j];
        }
    }
}

template<typename T>
Matrix<T>::~Matrix() {
    free();
}

template<typename T>
void Matrix<T>::free() {
    for (int i = 0; i < rows; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

template<typename T>
void Matrix<T>::output(std::ostream& out) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            out << data[i][j] << " ";
        }
        out << std::endl;
    }
}

template<typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> lst) {
    rows = lst.size();
    cols = lst.begin()->size();
    data = new T*[rows];
    int i = 0;
    for (auto it = lst.begin(), et = lst.end(); it != et; ++it) {
        data[i] = new T[cols];
        int j = 0;
        for (auto jt = it->begin(), jte = it->end(); jt != jte; ++jt) {
            data[i][j++] = *jt;
        }
        i++;
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
Matrix<T> Matrix<T>::product(const Matrix& B) {
    if (cols != B.rows) {
        return Matrix<T>();
    }
    Matrix<T> result(rows, B.cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < B.cols; ++j) {
            T sum{};
            for (int k = 0; k < cols; ++k) {
                sum += data[i][k]*B.data[k][j];
            }
            result[i][j] = sum;
            sum = T{};
        }
    }
    return result;
}
template<typename T>
Matrix<T> Matrix<T>::pow(int n) {
    Matrix<T> result(*this);
    if (n == 1) {
        return result;
    }
    for (int i = 1; i < n; ++i) {
        //Matrix<T> tmp(*this);
        result = result.product(*this);
        //result = tmp;
    }
    return result;

}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    free();
    copy(rhs);
    return *this;
}












#endif
