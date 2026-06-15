#include "Input.h"

#include "Application.h"

namespace Engine {
    bool Input::IsKeyPressed(KeyCode key) {
        const auto* window = Application::Get().GetWindow();
        auto* handle = window->GetHandle();

        const int state = glfwGetKey(handle, static_cast<int>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(MouseCode button) {
        const auto* window = Application::Get().GetWindow();
        auto* handle = window->GetHandle();

        const int state = glfwGetMouseButton(handle, static_cast<int>(button));
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::GetMousePosition() {
        const auto* window = Application::Get().GetWindow();
        auto* handle = window->GetHandle();

        double screenX = 0.0;
        double screenY = 0.0;
        glfwGetCursorPos(handle, &screenX, &screenY);

        return{
           static_cast<float>(screenX),
           static_cast<float>(screenY),
        };
    }

    float Input::ConsumeScrollXOffset() {
        auto offset = s_scrollXOffset;
        s_scrollXOffset = 0.0f;
        return offset;
    }

    float Input::ConsumeScrollYOffset() {
        auto offset = s_scrollYOffset;
        s_scrollYOffset = 0.0f;
        return offset;
    }

    void Input::AddMouseScrollOffset(float xOffset, float yOffset) {
        s_scrollXOffset += xOffset;
        s_scrollYOffset += yOffset;
    }
}
