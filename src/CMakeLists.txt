FLEX_TARGET(scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/scanner.hpp)
BISON_TARGET(parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp COMPILE_FLAGS "-d -v")
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
        ast/Type.cpp
        ast/Type.h
        ast/ParameterList.cpp
        ast/ParameterList.h
        ParserError.h
        ZppScanner.h
        ASTBuilder.cpp
        ASTBuilder.h
        Driver.cpp
        Driver.h
        Scope.cpp
        Scope.h
        Options.cpp
        Options.h
)

llvm_map_components_to_libnames(llvm_libs native nativecodegen orcjit)
target_link_libraries(z++ ${llvm_libs})
