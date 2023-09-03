#include <fstream>
#include <iostream>
#include <stdexcept>
#include <array>
#include <cmath>
#include "BitmapImage.h"

using std::cout;
using std::runtime_error;
using std::array;
using std::ifstream;
using std::ofstream;
using std::streamsize;
using dimension_type = BitmapImage::dimension_type;

static const int FILE_HEADER_SIZE = 14;
static const int DEFAULT_INFO_HEADER_SIZE = 40;

#pragma pack(push, 1)
struct FileHeader {
    uint16_t signature{0x4D42};       // Always 'BM'
    uint32_t fileSize{0};        // size of file in bytes
    uint32_t unused{0};
    uint32_t dataOffset{FILE_HEADER_SIZE + DEFAULT_INFO_HEADER_SIZE};      // offset from beginning of file to beginning of data
};
struct InfoHeader {
    uint32_t headerSize{DEFAULT_INFO_HEADER_SIZE};                      // size of information header in bytes
    int32_t width{0};                      // image width in pixels
    int32_t height{0};                     // image height in pixels, positive -> bottom-up, negative -> top-down
    uint16_t planes{1};                   // always 1
    uint16_t bitsPerPixel{24};                 // No. of bits per pixel
    uint32_t compression{0};               // compression mode
    uint32_t compressedImageSize{0};                // 0 - for uncompressed images
    int32_t xResolution{0};    // horizontal resolution
    int32_t yResolution{0};    // vertical resolution
    uint32_t colorsUsed{256};               // number of actually used colors
    uint32_t importantColors{0};
};
#pragma pack(pop)



BitmapImage::BitmapImage(const std::string &filename) {
    // open file and check for failure
    ifstream input{filename, std::ios_base::binary};
    if(!input){
        throw runtime_error("File could not be opened.");
    }

    // load file and information header, check their size
    FileHeader fileHeader{};
    InfoHeader infoHeader{};
    input.read(reinterpret_cast<char*>(&fileHeader), sizeof(FileHeader));
    if(input.gcount() != FILE_HEADER_SIZE){
        throw runtime_error("Wrong file format.");
    }
    input.read(reinterpret_cast<char*>(&infoHeader), sizeof(InfoHeader));
    if(input.gcount() != DEFAULT_INFO_HEADER_SIZE){
        throw runtime_error("Wrong file format.");
    }

    // check validity of file format
    bool offsetInvalid = fileHeader.dataOffset < FILE_HEADER_SIZE + DEFAULT_INFO_HEADER_SIZE;
    bool signatureInvalid = fileHeader.signature != 0x4D42;
    bool unsupportedColorFormat = infoHeader.bitsPerPixel != 24;
    bool compressed = infoHeader.compression != 0;
    bool negativeHeight = infoHeader.height < 0;

    if(offsetInvalid || signatureInvalid) throw runtime_error("Invalid file format.");
    if(unsupportedColorFormat) throw runtime_error("Unsupported color format.");
    if(compressed) throw runtime_error("Compressed images not supported.");
    if(negativeHeight) throw runtime_error("Input_images with negative height not supported.");

    // calculate padding
    int padding = ((4 - (abs(infoHeader.width) * 3) % 4) % 4);

    input.seekg(fileHeader.dataOffset, ifstream::beg);
    unsigned long long sizeInPixels = abs(infoHeader.width) * abs(infoHeader.height);
    pixels.resize(sizeInPixels);
    array<uint8_t, 3> buffer{};
    unsigned long long index = 0;

    for(unsigned long long row = 0; row < abs(infoHeader.height); row++){
        for(unsigned long long column = 0; column < abs(infoHeader.width); column++){
            input.read(reinterpret_cast<char*>(buffer.data()), 3);
            if(input.gcount() < 3){
                throw runtime_error("A problem occurred while reading the file.");
            }
            Color color(buffer[2], buffer[1], buffer[0]);
            pixels[index] = color;
            index++;
            input.ignore(padding);
        }
    }
    width = abs(infoHeader.width);
    height = abs(infoHeader.height);
    rowsReversed = false;
}

BitmapImage::BitmapImage(dimension_type w, dimension_type h) {
    width = w;
    height = h;
    pixels.resize(abs(w * h));
    if(height < 0) rowsReversed = true;
}

const Color& BitmapImage::getColor(dimension_type row, dimension_type column) const {
    return pixels[getIndex(row, column)];
}

// origin left upper
void BitmapImage::setColor(dimension_type row, dimension_type column, Color color) {
    pixels[getIndex(row, column)] = color;
}

void BitmapImage::writeToFile(const std::string &filename) const {
    struct FileHeader fileHeader;
    struct InfoHeader infoHeader;
    const int paddingSize = ((4 - (abs(infoHeader.width) * 3) % 4) % 4);
    ofstream output{filename, std::ios::binary};
    if(!output){
        throw runtime_error("File could not be opened.");
    }

    fileHeader.fileSize = FILE_HEADER_SIZE + DEFAULT_INFO_HEADER_SIZE + getHeight() * 3 * (getWidth() * 3 + paddingSize);
    infoHeader.width = width;
    infoHeader.height = height;
    infoHeader.compressedImageSize = pixels.size() * 3;


    output.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    output.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    uint8_t padding[paddingSize];
    for(int i = 0; i < paddingSize; i++){
        padding[i] = 0;
    }

    output.seekp(fileHeader.dataOffset, ofstream::beg);
    for(unsigned long long row = 0; row < height; row++){
        for(unsigned long long column = 0; column < getWidth(); column++){
            Color color = pixels[row * getWidth() + column];
            array<uint8_t, 3> buffer = {color.blue, color.green, color.red};
            output.write(reinterpret_cast<char*>(&buffer), 3);
        }
        output.write(reinterpret_cast<char*>(padding), paddingSize);
    }



}

std::vector<Color>::size_type
BitmapImage::getIndex(dimension_type row, dimension_type column) const {
    return rowsReversed ? (getHeight() - row - 1) * getWidth() + column : row * getWidth() + column;
}

void
BitmapImage::setColor(dimension_type row, dimension_type column, uint8_t red, uint8_t green, uint8_t blue) {
    int index = getIndex(row, column);
    pixels[index].red = red;
    pixels[index].green = green;
    pixels[index].blue = blue;
}
