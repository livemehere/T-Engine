file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_library(Core STATIC ${SOURCES})

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# TODO
#target_link_directories(Core)