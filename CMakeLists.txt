cmake_minimum_required(VERSION 3.31)
project(zmqdice LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nzmqt REQUIRED CONFIG)
find_package(Qt6 REQUIRED COMPONENTS Core Network)
qt_standard_project_setup()

qt_add_executable(zmqdice
    src/main.cpp
    src/benternet.cpp
)

target_link_libraries(zmqdice PRIVATE Qt6::Core Qt6::Network nzmqt)

qt_add_executable(dicetester
    src/tester.cpp
    src/benternet.cpp
)

target_link_libraries(dicetester PRIVATE Qt6::Core Qt6::Network nzmqt)
