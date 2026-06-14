cmake_minimum_required(VERSION 3.12)
project(tuner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_executable(tuner 
    "main.cpp" 
    "tuner.cpp" 
    "threadpool.cpp" 
    "engines/TuneEval.cpp"
    "engines/board.cpp"
    "engines/zobrist.cpp"
    "engines/evaluation.cpp"
    "engines/weights.cpp"
    "engines/adjustable_parameters.h"
)

target_link_libraries(tuner PRIVATE Threads::Threads)

target_include_directories(tuner PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/engine"
    "${CMAKE_CURRENT_SOURCE_DIR}/engines"
)

if(MSVC)
    target_compile_options(tuner PRIVATE
        $<$<CONFIG:Release>:/O2 /GL /DNDEBUG /arch:AVX2>
        $<$<CONFIG:Debug>:/Od /Zi>
    )
    target_link_options(tuner PRIVATE
        $<$<CONFIG:Release>:/LTCG>
        $<$<CONFIG:Debug>:/DEBUG>
    )
else()
    target_compile_options(tuner PRIVATE
        $<$<CONFIG:Release>:-O3 -march=native -DNDEBUG -flto>
        $<$<CONFIG:Debug>:-O0 -g>
    )
    target_link_options(tuner PRIVATE
        $<$<CONFIG:Release>:-flto>
    )
endif()