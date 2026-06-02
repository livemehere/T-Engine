#include "Application.h"
#include "Window.h"

int main() {

    Engine::AppSpec appSpec{
        .windowSpec = {
            .title = "Sample Window",
            .width = 1920,
            .height = 1080
        }
    };

    Engine::Application app{appSpec};

    app.Run();

    return 0;
}