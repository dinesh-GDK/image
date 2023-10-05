cmake_minimum_required(VERSION 3.10)

project(image_print)

# Add the include directories for the error_diffusion and dithering modules
include_directories(include)

# Add the main executable
add_executable(image_print src/main.cpp src/Image.cpp src/dithering.cpp src/error_diffusion.cpp)

# Set the build directory
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)

# Link the main executable to the error_diffusion and dithering modules
target_link_libraries(image_print -ljpeg)