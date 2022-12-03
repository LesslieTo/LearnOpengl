cmake_minimum_required(VERSION 3.5)

project(LearnOpenGL VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(EffectCore)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

#拷贝内部资源到app
#set(RESOURCES_PATH ${PROJECT_SOURCE_DIR}/EffectCore/Resources)
#set_source_files_properties(${RESOURCES_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
#file(COPY ${RESOURCES_PATH} DESTINATION "LearnOpenGL.app/Contents")

file(GLOB_RECURSE WIDGETS_SRC_LIST Widgets/*.cpp)
add_executable(LearnOpenGL MACOSX_BUNDLE
    main.cpp
    ${WIDGETS_SRC_LIST}
)

target_link_libraries(LearnOpenGL PRIVATE
    Qt5::Widgets
    EffectCore)


