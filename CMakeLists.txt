cmake_minimum_required(VERSION 3.5)
project(LightingProject)

add_subdirectory(lib/assimp)
add_subdirectory(lib/nanogui)

include_directories(lib/nanogui/include)
include_directories(lib/assimp/include)
include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/include)
include_directories(${NANOGUI_EXTRA_INCS})

add_definitions(${NANOGUI_EXTRA_DEFS})

add_executable(LightingProject main.cpp Camera.cpp Mesh.cpp Model.cpp Shader.cpp stb_image.cpp)

target_link_libraries(LightingProject nanogui ${NANOGUI_EXTRA_LIBS})
target_link_libraries(LightingProject assimp)