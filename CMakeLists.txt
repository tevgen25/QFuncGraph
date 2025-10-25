cmake_minimum_required(VERSION 3.10)
project(QFuncGraph VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_PREFIX_PATH "D:/Qt/Qt5.14.2/5.14.2/mingw73_64/lib/cmake")
find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_compile_definitions(MUPARSER_STATIC)

include_directories(${CMAKE_SOURCE_DIR}/lib/muparser/include)

add_executable(QFuncGraph
        main.cpp
        mainwindow.cpp
        qcustomplot.cpp
        lib/muparser/src/muParser.cpp
        lib/muparser/src/muParserBase.cpp
        lib/muparser/src/muParserBytecode.cpp
        lib/muparser/src/muParserError.cpp
        lib/muparser/src/muParserTokenReader.cpp
        lib/muparser/src/muParserCallback.cpp
)

target_link_libraries(QFuncGraph Qt5::Widgets Qt5::PrintSupport)

if(WIN32 AND NOT CMAKE_CROSSCOMPILING)
    find_program(WINDEPLOYQT NAMES windeployqt HINTS "${_qt_bin_dir}")
    if(WINDEPLOYQT)
        add_custom_command(TARGET QFuncGraph POST_BUILD
                COMMAND "${WINDEPLOYQT}" "$<TARGET_FILE:QFuncGraph>"
                COMMENT "Running windeployqt..."
        )
    endif()
endif()

