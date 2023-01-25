cmake_minimum_required(VERSION 3.0)
project(xylon)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -O2")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# Your-external "mylib", add GLOBAL if the imported library is located in directories above the current.
add_library( libbass SHARED IMPORTED )
add_library( libtag SHARED IMPORTED )

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_executable(xylon src/main.cpp src/Player.cpp src/gui/SongSearch.cpp src/gui/ProgressBarView.cpp src/SongView.cpp src/Song.cpp src/Tile.cpp src/gui/SongDisplay.cpp src/gui/VolumeCircleSlider.cpp src/gui/VolumeShape.cpp src/gui/Visualiser.cpp src/utils/Settings.cpp src/gui/SortSelect.cpp src/utils/PicLoader.cpp src/MainWindow.cpp
        src/utils/DirectoryNode.cpp src/gui/DirectorySelect.cpp src/gui/SelectView.cpp SelbaWard/Starfield.cpp src/utils/Utils.cpp
        src/gui/DownloadView.cpp src/gui/MouseTrace.cpp src/shaders/PostProcessing.cpp)

target_link_libraries(xylon tag)
target_link_libraries(xylon bass)
target_link_libraries(xylon sfml-graphics sfml-audio)
target_link_libraries(xylon Threads::Threads)
