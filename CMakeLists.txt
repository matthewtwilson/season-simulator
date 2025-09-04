cmake_minimum_required(VERSION 3.15)
project(season-simulator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the libraries
add_subdirectory(libs)

# Add the projects
add_subdirectory(projects/base)
add_subdirectory(projects/nfl)
add_subdirectory(projects/clubsoccer)
