file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# for glfw fast build
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(spdlog REQUIRED)
find_package(glm REQUIRED)

add_library(Engine STATIC ${SOURCES})

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(Engine PUBLIC
        OpenGL::GL
        glfw
        glad::glad
        spdlog::spdlog_header_only
        glm::glm
)

target_precompile_headers(Engine PRIVATE src/EngineCommon.h)