###############################################################################
##########          PROJECT
###############################################################################
cmake_minimum_required(VERSION 3.5)

project(LeakMan VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# To automatically run MOC when building(Meta Object Compiler)
set(CMAKE_AUTOMOC ON)

# To automatically run UIC when building(User Interface Compiler)
set(CMAKE_AUTOUIC ON)

# To automatically run RCC when building(Resource Compiler)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(GUI)
add_subdirectory(notGUI)
add_subdirectory(theLibForTest)

# Optimization flags for Release mode
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

#Les répertoires potentiels ou peut être opencv (mettre tous les path des dev dedans).
set(OPENCV_PATHS "C:/Users/sarah.mayer/Desktop/poc/poc_opencv/opencv-build" "C:/OpenCV" "C:/opencv/install/include" "C:/A_he_local/annee2/1_1_genieLogiciel/opencv-build")

find_package(OpenCV REQUIRED PATHS ${OPENCV_PATHS}) # Cherche le fichier opencv.cmake et l'execute. Explique à cmake quelles variables il doit initialiser et comment le faire fonctionner.
message(info ${OpenCV_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS} "C:/opencv/install/include")

set(PROJECT_SOURCES
        main.cpp
)

qt_add_resources(PROJECT_SOURCES resources.qrc)

#target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets ${OpenCV_LIBS})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LeakMan
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET LeakMan APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(LeakMan SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(LeakMan
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(LeakMan PUBLIC ${OpenCV_INCLUDE_DIRS}) # ou aller chercher les includes
target_link_libraries(LeakMan PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS} GUI notGUI theLibForTest) # Sur cette target leakman, ou aller chercher les librairy pour les link
set_target_properties(LeakMan PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LeakMan)
endif()

###############################################################################
##########          GOOGLETEST
###############################################################################
# GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
  )

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests tests.cpp ${notGUI_TESTS} ${theLibForTest_TESTS})
target_include_directories(tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests gtest_main notGUI theLibForTest)

include(GoogleTest)
gtest_discover_tests(tests)
enable_testing()

###############################################################################
##########          DOXYGEN
###############################################################################
#set(DOXYGEN_PATHS "C:/git_with_Fork/LeakManP2/LeakMan")

#find_package(Doxygen REQUIRED PATHS ${DOXYGEN_PATHS})
find_package(Doxygen)

if(NOT DOXYGEN_FOUND)
message(FATAL_ERROR "Doxygen is needed to build the documentation.")
endif()

set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

add_custom_target(BuildDoc ALL
  COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
  COMMENT "Generating Doxygen documentation"
  VERBATIM
)
