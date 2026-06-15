#pragma once

#include "KeyCode.h"
#include "MouseCode.h"

namespace Engine {
    class Input {
    public:
        static bool IsKeyPressed(KeyCode key);
        static bool IsMouseButtonPressed(MouseCode button);

        static glm::vec2 GetMousePosition();

        static float ConsumeScrollXOffset();
        static float ConsumeScrollYOffset();
        static void AddMouseScrollOffset(float xOffset, float yOffset);

    private:
        inline static float s_scrollXOffset = 0.0f;
        inline static float s_scrollYOffset = 0.0f;
    };


}
