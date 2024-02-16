cmake_minimum_required(VERSION 3.27)
project(z++)

set(CMAKE_CXX_STANDARD 20)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

FLEX_TARGET(scanner ${CMAKE_CURRENT_SOURCE_DIR}/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/scanner.hpp)
BISON_TARGET(parser ${CMAKE_CURRENT_SOURCE_DIR}/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp COMPILE_FLAGS "-d -v")
ADD_FLEX_BISON_DEPENDENCY(scanner parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(z++
        main.cpp
        ${FLEX_scanner_OUTPUTS}
        ${BISON_parser_OUTPUTS}
        ast/Expression.cpp
        ast/Expression.h
        ast/Statement.cpp
        ast/Statement.h
        ast/Program.h
        ast/Declaration.cpp
        ast/Declaration.h
        ParserError.h
        ZppScanner.h
        ASTBuilder.cpp
        ASTBuilder.h
        Driver.cpp
        Driver.h
        Scope.cpp
        Scope.h
)

llvm_map_components_to_libnames(llvm_libs native nativecodegen orcjit)
target_link_libraries(z++ ${llvm_libs})
