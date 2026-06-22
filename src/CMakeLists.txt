cmake_minimum_required(VERSION 3.12)
project(tuner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(mytuner
    main.cpp
    tuner.cpp
    threadpool.cpp
    engines/board.cpp
    engines/evaluation.cpp
    engines/TuneEval.cpp
    engines/weights.cpp
    engines/zobrist.cpp
)

target_include_directories(mytuner
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/engines
        ${CMAKE_CURRENT_SOURCE_DIR}/external
)

target_include_directories(mytuner PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/engine"
    "${CMAKE_CURRENT_SOURCE_DIR}/engines"
)

if(MSVC)
    target_compile_options(mytuner PRIVATE
        $<$<CONFIG:Release>:/O2 /GL /DNDEBUG /arch:AVX2>
        $<$<CONFIG:Debug>:/Od /Zi>
    )
    target_link_options(mytuner PRIVATE
        $<$<CONFIG:Release>:/LTCG>
        $<$<CONFIG:Debug>:/DEBUG>
    )
else()
    target_compile_options(mytuner PRIVATE
        $<$<CONFIG:Release>:-O3 -march=native -DNDEBUG -flto>
        $<$<CONFIG:Debug>:-O0 -g>
    )
    target_link_options(mytuner PRIVATE
        $<$<CONFIG:Release>:-flto>
    )
endif()