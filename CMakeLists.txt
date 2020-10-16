cmake_minimum_required(VERSION 3.16)

set(PROJ_NAME fast_rle)
set(EXE_NAME fast_rle_exe)
set(LIB_NAME fast_rle_lib)
set(Boost_USE_MULTITHREADED TRUE)
set(CMAKE_SHARED_MODULE_PREFIX "")
set(CMAKE_CXX_STANDARD 20)

project(${PROJ_NAME} LANGUAGES C CXX)

if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_CXX_FLAGS "-fPIC -m64 -Ofast -mtune=native -march=native -mfpmath=sse -flto -funroll-loops -fno-signed-zeros -fno-trapping-math" ) #-fexperimental-new-pass-manager -flto=thin")
endif ()

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

find_package (Python COMPONENTS Interpreter Development NumPy)

message(STATUS "PYTHON_LIBRARIES = ${Python_LIBRARIES}")
message(STATUS "PYTHON_EXECUTABLE = ${Python_EXECUTABLE}")
message(STATUS "PYTHON_INCLUDE_DIRS = ${Python_INCLUDE_DIRS}")
message(STATUS "CONAN_LIBS = ${CONAN_LIBS}")
message(STATUS "BINARY_DIR = ${CMAKE_BINARY_DIR}")


set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/fast_rle)
set(CSV_PARSER_DIR ${SRC_DIR}/fast-cpp-csv-parse)
set(PT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/python_stuff)


list(APPEND SRC_FILES ${SRC_DIR}/rle_file.cpp
        ${SRC_DIR}/log_latency.cpp
        ${SRC_DIR}/to_parts.h
        ${SRC_DIR}/shared_utils.cpp
        ${SRC_DIR}/csv_utils.cpp
        ${SRC_DIR}/decode_utils.cpp
        ${SRC_DIR}/encode_utils.cpp
        ${SRC_DIR}/save_utils.cpp
        ${SRC_DIR}/masks_utils.cpp
        ${SRC_DIR}/csv_writer.hpp
        )

list(APPEND HEADERS_FILES ${SRC_DIR}/rle_file.h
        ${SRC_DIR}/to_parts.h
        ${SRC_DIR}/shared_utils.h
        ${SRC_DIR}/csv_utils.h
        ${SRC_DIR}/decode_utils.h
        ${SRC_DIR}/encode_utils.h
        ${SRC_DIR}/save_utils.h
        ${SRC_DIR}/masks_utils.h
        ${SRC_DIR}/csv_writer.hpp
        )


list(APPEND PT_SRC_FILES ${PT_SRC_DIR}/rle_file_wrapper.cpp
        ${PT_SRC_DIR}/python_utils.cpp
        ${PT_SRC_DIR}/converters.cpp
        )
list(APPEND PT_HEADES_FILES ${PT_SRC_DIR}/rle_file_wrapper.h
        ${PT_SRC_DIR}/python_utils.h
        ${PT_SRC_DIR}/converters.h
        )

set(CSV_PARSER_FILES ${CSV_PARSER_DIR}/csv.h)


add_executable(${EXE_NAME} fast_rle_exe.cpp ${HEADERS_FILES}
        ${SRC_FILES}
        ${CSV_PARSER_FILES}
        )
target_link_libraries(${EXE_NAME} PRIVATE ${CONAN_LIBS})
target_include_directories(${EXE_NAME} PRIVATE ${SRC_DIR})
set_property(TARGET ${EXE_NAME} PROPERTY CXX_STANDARD 17)

Python_add_library(${LIB_NAME} MODULE fast_rle.cpp ${PT_SRC_FILES}
        ${PT_HEADERS_FILES}
        ${SRC_FILES}
        ${HEADERS_FILES}
        ${CSV_PARSER_FILES}
        )

target_link_libraries(${LIB_NAME} PRIVATE ${CONAN_LIBS})
target_include_directories(${LIB_NAME} PRIVATE src)

install(TARGETS ${LIB_NAME}
        CONFIGURATIONS Release
        DESTINATION ${CMAKE_SOURCE_DIR}/fast_rle
        )