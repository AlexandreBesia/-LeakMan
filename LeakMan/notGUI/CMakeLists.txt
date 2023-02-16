set(TARGET_NAME notGUI)

set(CMAKE_AUTOMOC ON)

set(${TARGET_NAME}_SOURCES
    block.cpp
    block.h
    level.cpp
    level.h
    levelgrid.cpp
    levelgrid.h
    physicscomponent.cpp
    physicscomponent.h
)

set(${TARGET_NAME}_TESTS
${CMAKE_CURRENT_SOURCE_DIR}/notGUI_tests.cpp

PARENT_SCOPE # must be reused from main testing program
)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(OPENCV_PATHS "C:/Users/sarah.mayer/Desktop/poc/poc_opencv/opencv-build" "C:/OpenCV" "C:/opencv/install/include" "C:/A_he_local/annee2/1_1_genieLogiciel/opencv-build")

find_package(OpenCV REQUIRED PATHS ${OPENCV_PATHS})
include_directories(${OpenCV_INCLUDE_DIRS} "C:/opencv/install/include")

add_library(${TARGET_NAME} ${${TARGET_NAME}_SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC ..)
target_link_libraries(${TARGET_NAME} Qt6::Core Qt6::Gui Qt6::Widgets ${OpenCV_LIBS} GUI)
