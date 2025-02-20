file(GLOB INC_FILES "*.h")
file(GLOB SRC_FILES "src/*.cpp")

set(EXECUTABLE_NAME "nfl")

add_executable(${EXECUTABLE_NAME}
    ${INC_FILES}
    ${SRC_FILES}
)

# Include the project-specific include directory
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link the shared libraries from lib/
target_link_libraries(${EXECUTABLE_NAME} PRIVATE common)
