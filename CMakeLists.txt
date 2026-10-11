cmake_minimum_required(VERSION 4.2.3)

project(Sparky LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    #add_compile_options(-fsanitize=address)

    #add_link_options(-fsanitize=address)

add_executable(Sparky
    main.cpp
    src/Math.cpp    
    src/Lexer.cpp
    src/Parser.cpp
    src/FileReader.cpp
    src/AST.cpp
    src/ASTPrinter.cpp
)

target_link_libraries(Sparky
    PRIVATE
)

target_include_directories(Sparky
    PRIVATE
        ${CMAKE_SOURCE_DIR}
)