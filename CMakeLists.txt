cmake_minimum_required(VERSION 3.0)

project(MyProject)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules ${CMAKE_MODULE_PATH})

find_package(Pythia8)
find_package(HepMC3)
find_package(ROOT REQUIRED)
find_package(TBB)

add_executable(testMultiThread testMultiThread.cpp)

target_link_libraries(testMultiThread PUBLIC Pythia8 ROOT::Tree ROOT::RIO ROOT::Core HepMC3::HepMC3 HepMC3::rootIO TBB::tbb)
