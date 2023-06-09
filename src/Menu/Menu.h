#ifndef RAGNAROK_MENU_H
#define RAGNAROK_MENU_H

#include <string>
#include <chrono>

#include "glfw/imports.h"

class Menu {
private:
    std::string m_title;
    int m_width, m_height;
    GLFWwindow* m_window;
    bool m_running = false;
    bool m_open = true;
private:
    std::chrono::time_point<std::chrono::system_clock> m_lastRenderTime;
private:
    bool m_InitGLFW();
    bool m_CreateWindow();
public:
    Menu(std::string title, int width, int height);
    ~Menu();

    void Render();

    bool IsRunning();
    void ToggleOpen();
}; // class Menu

#endif