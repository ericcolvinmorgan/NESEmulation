cmake_minimum_required(VERSION 3.5)
project(nesemulation VERSION 0.1.0)

# include(CTest)
# enable_testing()

add_subdirectory(src/emulator)
add_subdirectory(src/wasm)
add_subdirectory(test)

target_include_directories(emulator PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

                           

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
