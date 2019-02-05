#########################
# Tree for this file should be:
#
# src/            (*.cpp)
# inc/            (*.h)
# build/          (build files)
# CMakeLists.txt  (this file)
#
# Leave '#_files' in place for an outside run script
#
# Run cmake from within 'build/'
# Run make from within 'build/'
# Run the target from within 'build/'
#########################

# cmake version
cmake_minimum_required(VERSION 3.2)

# project name
project(glw)

# set the include directory for header files
set(INC ${PROJECT_SOURCE_DIR}/inc)
include_directories(${INC})

# set the source directory for the source files
set(SRC ${PROJECT_SOURCE_DIR}/src)

# specifiy each source file to be compiled 
set(SRC_FILES
#files
${SRC}/GGUI.cpp
${SRC}/ImageLoader.cpp
${SRC}/GFrameBufferObject.cpp
${SRC}/GCamera.cpp
${SRC}/GPrimativeFactory.cpp
${SRC}/GVertexBufferObject.cpp
${SRC}/GShaderProgramManager.cpp
${SRC}/GShaderVariableHandle.cpp
${SRC}/GShaderProgram.cpp
${SRC}/GContent.cpp
${SRC}/main.cpp
)

# link source files to the project already specified
add_executable(glw ${SRC_FILES})

# link libraries to the project to compile
target_link_libraries(glw PUBLIC GL GLU GLEW glfw3 X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor)
