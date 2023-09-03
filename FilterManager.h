//
// Created by klara on 26.03.2023.
//

#include "BitmapImage.h"
#include "Matrix.h"

#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H


class FilterManager {
public:
    explicit FilterManager(BitmapImage& i): image{i} {};
    BitmapImage& getImage() {return image;};
    void convertToGrayScale();
    void detectEdges();
private:
    BitmapImage& image;
};


#endif
