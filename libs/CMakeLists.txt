file(GLOB INC_FILES "common/*.h")
file(GLOB SRC_FILES "common/src/*.cpp")

add_library(common STATIC
    ${INC_FILES}
    ${SRC_FILES}
)

# Make sure these libraries are available to dependents
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
