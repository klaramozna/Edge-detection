//
// Created by klara on 26.03.2023.
//

#include "FilterManager.h"
#include "Matrix.h"
#include <stdexcept>
#include <cmath>
#include <algorithm>

uint8_t getGrayscaleValue(const Color& color){
    return (color.red + color.green + color.blue) / 3;
}

void FilterManager::convertToGrayScale() {
    for(unsigned long long row = 0; row < image.getHeight(); row++){
        for(unsigned long long column = 0; column < image.getWidth(); column++){
            uint8_t value = getGrayscaleValue(image.getColor(row, column));
            image.setColor(row, column, value, value, value);
        }
    }
}

const int KERNEL_SIZE = 3;

Matrix imageToMatrix(int x, int y, int width, int height, const BitmapImage& image){
    Matrix m(width, height);
    for(int row = 0; row < height; row++){
        for(int column = 0; column < width; column++){
            int imageX = x - width/2 + column;
            int imageY = y - height/2 + row;
            if(imageX < 0 || imageX >= image.getWidth() || imageY < 0 || imageY >= image.getHeight()){
                m.setValue(row, column, 0);
            }
            else{
                m.setValue(row, column, image.getColor(imageY, imageX).blue);
            }
        }
    }
    return m;
}

const double PERCENTILE = 0.96;

void FilterManager::detectEdges() {
    BitmapImage copy = image;
    std::vector<double> kernelXVec{1, 0, -1, 2, 0, -2, 1, 0, -1};
    std::vector<double> kernelYVec{-1, -2, -1, 0, 0, 0, 1, 2, 1};
    Matrix kernelX(kernelXVec.begin(), kernelXVec.end(), KERNEL_SIZE, KERNEL_SIZE);
    Matrix kernelY(kernelYVec.begin(), kernelYVec.end(), KERNEL_SIZE, KERNEL_SIZE);
    Matrix gradientX(KERNEL_SIZE, KERNEL_SIZE);
    Matrix gradientY(KERNEL_SIZE, KERNEL_SIZE);
    std::vector<int> values;
    values.reserve(image.getWidth() * image.getHeight());
    for(int row = 0; row < image.getHeight(); row++){
        for(int column = 0; column < image.getWidth(); column++){
            // construct matrix from this part of image
            if(row == 0 || column == 0 || row == image.getHeight() - 1 || column == image.getWidth() - 1){
                image.setColor(row, column, Color(0, 0, 0));
            }
            else{
                gradientX = gradientY = imageToMatrix(column, row, KERNEL_SIZE, KERNEL_SIZE, copy);

                int value = (int) sqrt(pow(gradientX.convolve(kernelX), 2) + pow(gradientY.convolve(kernelY), 2));
                image.setColor(row, column, Color(value, value, value));
                values.push_back(value);
            }
        }
    }

    std::sort(values.begin(), values.end());
    unsigned long long index = (PERCENTILE * (double) image.getHeight() * (double) image.getWidth());
    int threshold = values[index];

    for(int row = 0; row < image.getHeight(); row++){
        for(int column = 0; column < image.getWidth(); column++){
            if(image.getColor(row, column).blue >= threshold){
                image.setColor(row, column, Color(255, 255, 255));
            }
            else{
                image.setColor(row, column, Color(0, 0, 0));
            }
        }
    }


}






