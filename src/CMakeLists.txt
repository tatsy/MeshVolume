set(BUILD_TARGET "demo")

file(GLOB SOURCE_FILES "*.cpp" "*h")

add_executable(${BUILD_TARGET} ${SOURCE_FILES})
source_group("Source Files" FILES ${SOURCE_FILES})
