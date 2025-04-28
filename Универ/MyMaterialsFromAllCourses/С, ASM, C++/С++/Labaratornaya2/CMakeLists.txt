cmake_minimum_required(VERSION 3.24)
project(Labaratornaya2)

set(CMAKE_CXX_STANDARD 17)

add_executable(Labaratornaya2 main.cpp PassengerTransport.cpp PassengerTransport.h Airplane.cpp Airplane.h Train.cpp Train.h Car.cpp Car.h)
