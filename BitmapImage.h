#ifndef BITMAPIMAGE_H
#define BITMAPIMAGE_H

#include <cstdint>
#include <string>
#include <vector>
#include "Color.h"

class BitmapImage {

public:
    typedef int32_t dimension_type;
    typedef std::vector<Color>::iterator iterator;
    typedef std::vector<Color>::const_iterator const_iterator;
    iterator begin(){return pixels.begin();};
    iterator end(){return pixels.end();};
    const_iterator cbegin(){return pixels.cbegin();};
    const_iterator cend(){return pixels.cend();};
    BitmapImage(): pixels(), height(0), width(0), rowsReversed(false){};
    explicit BitmapImage(const std::string& filename);
    BitmapImage(dimension_type width, dimension_type height);
    const Color& getColor(dimension_type row, dimension_type column) const;
    void setColor(dimension_type row, dimension_type column, Color color);
    void setColor(dimension_type row, dimension_type column, uint8_t red, uint8_t green, uint8_t blue);
    void writeToFile(const std::string& filename) const;
    uint32_t getWidth() const {return abs(width);};
    uint32_t getHeight() const {return abs(height);};
    class it{
    public: int hello;
    };
private:
    std::vector<Color> pixels;
    dimension_type height;
    dimension_type width;
    bool rowsReversed;
    std::vector<Color>::size_type getIndex(dimension_type row, dimension_type column) const;
};

#endif
