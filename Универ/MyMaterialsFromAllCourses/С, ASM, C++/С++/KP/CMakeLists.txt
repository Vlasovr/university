cmake_minimum_required(VERSION 3.25)
project(KP)

set(CMAKE_CXX_STANDARD 17)

add_executable(KP main.cpp FileProcessor.h Node.h TemplateList.h ListWithIterator.h Student.h InterfaceWithList.h PassengerTransport.cpp PassengerTransport.h InterfaceWithFile.h Functions.h InterfaceWithTemplateList.cpp InterfaceWithTemplateList.h TemplateList.cpp ListWithIterator.cpp InterfaceWithList.cpp Algorithms.cpp Algorithms.h)
