#include "GuiLayer.h"

#include "Renderer2D.h"
#include "Core/Application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

GuiLayer::GuiLayer(const std::string& name, std::shared_ptr<Engine::OrthographicCamera> camera)
    :Layer(name), camera(std::move(camera)) {
    auto window = Engine::Application::Get().GetWindow();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window->GetHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 410 core");
}

GuiLayer::~GuiLayer() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GuiLayer::OnUpdate(float dt) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GuiLayer::OnRender() {
    auto window = Engine::Application::Get().GetWindow();
    auto* handle = window->GetHandle();

    auto screenWidth = static_cast<float>(window->GetWidth());
    auto screenHeight = static_cast<float>(window->GetHeight());

    double screenX = 0.0;
    double screenY = 0.0;
    glfwGetCursorPos(handle, &screenX, &screenY);

    const glm::vec3 worldCursor = camera->ScreenToWorld({screenX, screenY}, {screenWidth, screenHeight});

    ImGui::Begin("Mouse");
    ImGui::Text("World : %.1f x %.1f", worldCursor.x, worldCursor.y);
    ImGui::End();

    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    drawList->AddCircle(ImVec2(screenX, screenY), 10.0f * camera->GetZoom(), IM_COL32(0, 255, 0, 255), 232, 2.0f);

    ImGui::Begin("Stats");
    ImGui::SetWindowFontScale(1.5f);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    auto stats = Engine::Renderer2D::GetStats();
    ImGui::Text("Draw Calls: %d", stats.drawCalls);
    ImGui::Text("Rect Count: %d", stats.rectCount);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GuiLayer::OnAttach() {
    LOG_INFO("Attach GUI Layer");
}

void GuiLayer::OnDetach() {
    LOG_INFO("Detach GUI Layer");
}
