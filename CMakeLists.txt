cmake_minimum_required(VERSION 3.20)

project(
    FHP1LatticeGasSimulation
    VERSION 0.1.0
    DESCRIPTION "FHP-1 lattice gas simulation in C with MPI"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include(CTest)

add_library(fhp1_core
    src/lattice.c
)

target_include_directories(fhp1_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(fhp1_core PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

if(BUILD_TESTING)
    add_executable(fhp1_tests
        tests/test_lattice.c
    )

    target_link_libraries(fhp1_tests
        PRIVATE fhp1_core
    )

    if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(fhp1_tests PRIVATE
            -Wall
            -Wextra
            -Wpedantic
        )
    endif()

    add_test(
        NAME lattice_tests
        COMMAND fhp1_tests
    )

    add_executable(fhp1_state_tests tests/test_state.c)
    target_link_libraries(fhp1_state_tests PRIVATE fhp1_core)

    if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(fhp1_state_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    add_test(NAME state_tests COMMAND fhp1_state_tests)
endif()