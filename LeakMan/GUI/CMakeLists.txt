set(TARGET_NAME GUI)

set(CMAKE_AUTOMOC ON)

set(${TARGET_NAME}_SOURCES
    actor.h
    calibrationstate.cpp
    calibrationstate.h
    calibrationwindow.cpp
    calibrationwindow.h
    component.cpp
    component.h
    componenttypeconverter.h
    creditsstate.cpp
    creditsstate.h
    editorstate.cpp
    editorstate.h
    gamestate.cpp
    gamestate.h
    levelselectionstate.cpp
    levelselectionstate.h
    levelstate.cpp
    levelstate.h
    mainwindow.cpp
    mainwindow.h
    menustate.cpp
    menustate.h
    state.cpp
    state.h
    stateid.h
    statestack.cpp
    statestack.h
    stickman.cpp
    stickman.h
    gamewindow.h
    gamewindow.cpp
    victorystate.cpp
    victorystate.h
)

set(${TARGET_NAME}_TESTS
${CMAKE_CURRENT_SOURCE_DIR}/GUI_tests.cpp

PARENT_SCOPE # must be reused from main testing program
)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(OPENCV_PATHS "C:/Users/sarah.mayer/Desktop/poc/poc_opencv/opencv-build" "C:/OpenCV" "C:/opencv/install/include" "C:/A_he_local/annee2/1_1_genieLogiciel/opencv-build")

find_package(OpenCV REQUIRED PATHS ${OPENCV_PATHS})
include_directories(${OpenCV_INCLUDE_DIRS} "C:/opencv/install/include")

message(STATUS "TEST")

add_library(${TARGET_NAME} ${${TARGET_NAME}_SOURCES})

message(STATUS ${TARGET_NAME}_SOURCES)
message(STATUS "=========")
message(${GUI_SOURCES})
message(STATUS "TEST2")

target_include_directories(${TARGET_NAME} PUBLIC ..)
target_link_libraries(${TARGET_NAME} Qt6::Core Qt6::Gui Qt6::Widgets ${OpenCV_LIBS} notGUI)
