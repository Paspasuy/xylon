cmake_minimum_required(VERSION 3.0)
project(xylon)

set(CMAKE_CXX_STANDARD 20)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# Your-external "mylib", add GLOBAL if the imported library is located in directories above the current.
add_library( libbass SHARED IMPORTED )
add_library( libtag SHARED IMPORTED )

add_executable(xylon src/main.cpp src/Player.cpp src/SongSearch.cpp src/ProgressBarView.cpp src/SongView.cpp src/Song.cpp src/Tile.cpp src/SongDisplay.cpp src/VolumeCircleSlider.cpp SelbaWard/Starfield.cpp)

target_link_libraries(xylon tag)
target_link_libraries(xylon bass)
target_link_libraries(xylon sfml-graphics sfml-audio)