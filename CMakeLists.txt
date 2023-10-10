cmake_minimum_required(VERSION 3.5)
project(HyperShadow)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

find_package(
  VTK
  COMPONENTS CommonCore
             GUISupportQt
             IOImage
             ImagingColor
             ImagingGeneral
             InteractionImage
             InteractionStyle
             InteractionWidgets
             RenderingCore
             RenderingOpenGL2
             RenderingVolume
             RenderingAnnotation
             RenderingVolumeOpenGL2)
if(NOT VTK_FOUND)
  message("${VTK_NOT_FOUND_MESSAGE}")
  return()
endif()

# Find the Qt5 package
find_package("Qt${VTK_QT_VERSION}" REQUIRED COMPONENTS Widgets)

if(NOT TARGET "Qt${VTK_QT_VERSION}::Widgets")
  message("${Qt${VTK_QT_VERSION}_NOT_FOUND_MESSAGE}")
  return()
endif()

# Set the source files
set(Srcs
    source/Application.cpp source/HyperPoints.cpp source/HyperCube.cpp
    source/HyperCubeProjectionActor.cpp source/HyperPlane.cpp source/main.cpp)

set(Hdrs source/Application.hpp source/HyperPoints.hpp source/HyperCube.hpp
         source/HyperCubeProjectionActor.hpp source/HyperPlane.hpp)
#
# set(UIs QtVTKRenderWindows.ui)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# CMAKE_AUTOMOC in ON so the MocHdrs will be automatically wrapped.
add_executable(${PROJECT_NAME} ${Srcs} ${Hdrs} ${UIs})
target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES}
                                              "Qt${VTK_QT_VERSION}::Widgets")
vtk_module_autoinit(TARGETS HyperShadow MODULES ${VTK_LIBRARIES})
