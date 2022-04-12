include_directories(.)
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c*)

add_executable(Question2.cpp ${SRC})