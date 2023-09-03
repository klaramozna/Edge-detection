cmake_minimum_required(VERSION 3.23)
project(Projekt_Sobel_Edge_Detection)

set(CMAKE_CXX_STANDARD 14)

add_executable(Projekt_Sobel_Edge_Detection main.cpp BitmapImage.cpp BitmapImage.h Color.cpp Color.h Matrix.cpp Matrix.h FilterManager.cpp FilterManager.h)
