set(TARGET_NAME theLibForTest)

set(${TARGET_NAME}_SOURCES 
liba_1.h
liba_1.cpp
)
set(${TARGET_NAME}_TESTS
${CMAKE_CURRENT_SOURCE_DIR}/theLibForTest_tests.cpp

PARENT_SCOPE # must be reused from main testing program
)

add_library(${TARGET_NAME} ${${TARGET_NAME}_SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC ..)
