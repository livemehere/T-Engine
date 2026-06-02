file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(App ${SOURCES})

target_link_libraries(App PRIVATE Core)