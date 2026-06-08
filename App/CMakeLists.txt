file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

find_package(imgui CONFIG REQUIRED)

add_executable(App ${SOURCES})

target_link_libraries(App PRIVATE
        Engine
        imgui::imgui
)