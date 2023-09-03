#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <stdexcept>

class Matrix {
public:
    Matrix(): values() {};
    Matrix(int width, int height);
    template<class Container>
    Matrix(Container begin, Container end, int w, int h);
    double getValue(int i, int y) const;
    void setValue(int i, int y, double value);
    int getSize() const;
    double convolve(const Matrix&);
private:
    std::vector<double> values;
    bool dimensionsMatch(const Matrix&) const;
    int height;
    int width;
};

template<class Container>
Matrix::Matrix(Container begin, Container end, int w, int h) {
    if(end-begin != w * h){
        throw std::runtime_error("Size of container does not match given dimensions.");
    }
    values.reserve(w * h);
    for(Container i = begin; i != end; i++){
        values.push_back(*i);
    }
    height = h;
    width = w;
}




#endif
