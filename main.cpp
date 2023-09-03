#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <chrono>
#include "Color.h"
#include "BitmapImage.h"
#include "FilterManager.h"
#include "Color.h"

using std::cout;
using std::endl;

Matrix hello(int x, int y, int width, int height, const BitmapImage& image){
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

int main() {
    BitmapImage f(R"(C:\Users\klara\CLionProjects\Projekt-Sobel_Edge_Detection\Input_images\horse.bmp)");
    FilterManager m(f);
    m.convertToGrayScale();
    m.detectEdges();
    f.writeToFile(R"(C:\Users\klara\CLionProjects\Projekt-Sobel_Edge_Detection\Output_images\test.bmp)");




}
