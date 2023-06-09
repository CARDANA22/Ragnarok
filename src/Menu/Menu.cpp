#include "Menu.h"

#include <iostream>
#include <thread>

#include "../Modules/Manager/ModuleManager.h"

#include "../Modules/NoRecoil/NoRecoil.h"
#include "../Modules/Spectatorcount/Spectatorcount.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../Keys/KeyManager.h"

bool isOpen = true;

void Toggle() {
    isOpen = !isOpen;
}

Menu::Menu(std::string title, int width, int height) {
    this->m_title = title;

    this->m_width = width;
    this->m_height = height;

    this->m_lastRenderTime = std::chrono::high_resolution_clock::now(); 

    KeyManager::AddListener(0xffbf, &Toggle);

    if(Menu::m_InitGLFW())
        this->m_running = true;
}

Menu::~Menu() {
    glfwDestroyWindow(this->m_window);
    glfwTerminate();
}

bool Menu::m_InitGLFW() {
    if(!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_REFRESH_RATE, 144);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    if(!this->m_CreateWindow())
        return false;

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(this->m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool Menu::m_CreateWindow() {
    this->m_window = glfwCreateWindow(this->m_width, this->m_height, this->m_title.c_str(), NULL, NULL);

    if(!this->m_window)
        return false;

    glfwMakeContextCurrent(this->m_window);
    glfwSetFramebufferSizeCallback(this->m_window, framebuffer_size_callback);

    glfwSetWindowAttrib(this->m_window, GLFW_DECORATED, GLFW_FALSE);
    glfwSetWindowAttrib(this->m_window, GLFW_MOUSE_PASSTHROUGH, GLFW_FALSE);
    glfwSetWindowAttrib(this->m_window, GLFW_FLOATING, GLFW_TRUE);

    glfwSwapInterval(0);

    return true;
}

bool RecoilControl = false;
bool Glow = false;

float minRecoilBuffer = ModuleManager::minRecoil;
float maxRecoilBuffer = ModuleManager::maxRecoil;

float visibleBufffer[3] = {0.f, 1.f, 0.f};
float hiddenBuffer[3] = { 1.f, 0.f, 0.f};

const char* types[] = { "Health", "Teams", "Color" };
const char* currentItem = types[2];

void Menu::Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if(isOpen && glfwGetWindowAttrib(this->m_window, GLFW_MOUSE_PASSTHROUGH) == GLFW_TRUE) {
        glfwSetWindowAttrib(this->m_window, GLFW_MOUSE_PASSTHROUGH, GLFW_FALSE);
    } else if(!isOpen && glfwGetWindowAttrib(this->m_window, GLFW_MOUSE_PASSTHROUGH) == GLFW_FALSE) {
        glfwSetWindowAttrib(this->m_window, GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
        glfwIconifyWindow(this->m_window);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        glfwShowWindow(this->m_window);
    }

    ImGui::GetForegroundDrawList()->AddCircle(ImVec2(this->m_width * 0.5, this->m_height * 0.5 - 32.f), 10.f, ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f, 1.f)));

    if(isOpen) {
        //ImGui::ShowDemoWindow();
        ImGui::Begin("Ragnarok", NULL, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Checkbox("Recoil Control", &RecoilControl);
            
        if(RecoilControl) {
            ModuleManager::EnableModule("NoRecoil");

            if(ImGui::SliderFloat("Min", &minRecoilBuffer, 0.f, 100.f) ||
                ImGui::SliderFloat("Max", &maxRecoilBuffer, 0.f, 100.f)) {
                if(maxRecoilBuffer < minRecoilBuffer)
                    maxRecoilBuffer = minRecoilBuffer;

                ModuleManager::minRecoil = minRecoilBuffer;
                ModuleManager::maxRecoil = maxRecoilBuffer;
            }
        }
        else if(!RecoilControl)
            ModuleManager::DisableModule("NoRecoil");

        ImGui::Checkbox("Glow", &Glow);

        if(Glow) {
            ModuleManager::EnableModule("Glow");

            if(ImGui::BeginCombo("Type", currentItem)) {
                for(int i = 0; i < IM_ARRAYSIZE(types); i++) {
                    bool isSelected = (strcmp(currentItem, types[i]) == 0);

                    if(ImGui::Selectable(types[i], isSelected))
                        currentItem = types[i];

                    if(isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            if(strcmp(currentItem, "Color") == 0) {
                ModuleManager::healthBased = false;

                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.83);
                if(ImGui::ColorEdit3("Visible", visibleBufffer)) {
                    ModuleManager::visible = Vector3{visibleBufffer[0], visibleBufffer[1], visibleBufffer[2]};
                }

                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.83);
                if(ImGui::ColorEdit3("Hidden", hiddenBuffer)) {
                    ModuleManager::hidden = Vector3{hiddenBuffer[0], hiddenBuffer[1], hiddenBuffer[2]};
                }
            }
            else if(strcmp(currentItem, "Health") == 0)
            {
                ModuleManager::healthBased = true;
            }
        }else if(!Glow)
            ModuleManager::DisableModule("Glow");

        ImGui::Text(std::string("FPS: ").append(std::to_string(ImGui::GetIO().Framerate)).c_str());
        ImGui::Text(std::string("Spectator: ").append(std::to_string(ModuleManager::spectatorcount)).c_str());

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(this->m_window);

    glfwPollEvents();

    if(glfwWindowShouldClose(this->m_window))
        this->m_running = false;
}

bool Menu::IsRunning() {
    return this->m_running;
}

void Menu::ToggleOpen() {
    this->m_open = !this->m_open;
}