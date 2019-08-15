
get_filename_component(DirName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" DirrName ${DirName})

set(OUTPUT ${DirName})

add_executable(${OUTPUT} main.cpp )



