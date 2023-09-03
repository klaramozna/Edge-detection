//
// Created by klara on 25.03.2023.
//

#include <stdexcept>
#include "Matrix.h"

using std::out_of_range;

Matrix::Matrix(int w, int h) {
    values.resize(w * h);
    std::fill(values.begin(), values.end(), 0);
    width = w;
    height = h;
}



// origin in left upper corner
double Matrix::getValue(int row, int column) const {
    if(row >= height || row < 0 || column >= width || column < 0){
        throw out_of_range("Index out of range.");
    }
    return values[row * width + column];
}

bool Matrix::dimensionsMatch(const Matrix & m) const {
    return width == m.width && height == m.height;
}

int Matrix::getSize() const {
    return width * height;
}

double Matrix::convolve(const Matrix & m) {
    if(!dimensionsMatch(m)){
        throw std::runtime_error("Dimensions do not match.");
    }
    double sum = 0;
    for(int i = 0; i < values.size(); i++){
        sum += values[i] * m.values[i];
    }
    return sum;
}

void Matrix::setValue(int row, int column, double value) {
    if(row >= height || row < 0 || column >= width || column < 0){
        throw out_of_range("Index out of range.");
    }
    values[row * width + column] = value;
}



