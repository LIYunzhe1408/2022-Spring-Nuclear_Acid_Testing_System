include_directories(.)
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c*)

aux_source_directory(./COVID_19_TestSystem CPP_LIST)
aux_source_directory(./Personnel CPP_LIST2)
aux_source_directory(./TestTube CPP_LIST3)
add_executable(Question2.cpp ${SRC}  ${CPP_LIST2} ${CPP_LIST3} ${CPP_LIST})