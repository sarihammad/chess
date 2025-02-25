cmake_minimum_required(VERSION 3.15)
project(Chessmate)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(SFML 2.6.1 COMPONENTS graphics window system audio REQUIRED)
find_package(Torch REQUIRED)
find_package(websocketpp REQUIRED)
find_package(JsonCpp REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${TORCH_INCLUDE_DIRS})

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(chessmate ${SOURCES})

# Link libraries
target_link_libraries(chessmate 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio
    ${TORCH_LIBRARIES}
    jsoncpp
) 