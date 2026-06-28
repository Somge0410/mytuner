cmake_minimum_required(VERSION 3.12)
project(tuner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find Threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
 
# 1. FIXED: Added 'src/' to all paths based on your folder structure
add_executable(mytuner
    src/main.cpp
    src/tuner.cpp
    src/threadpool.cpp
    src/engines/board.cpp
    src/engines/evaluation.cpp
    src/engines/TuneEval.cpp
    src/engines/weights.cpp
    src/engines/zobrist.cpp
)

# 2. FIXED: Link the Threads package to the executable
target_link_libraries(mytuner PRIVATE Threads::Threads)

# 3. FIXED: Consolidated include directories and pointed them to 'src'
target_include_directories(mytuner PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engines
    ${CMAKE_CURRENT_SOURCE_DIR}/src/external
)

# Compiler and Linker optimizations
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