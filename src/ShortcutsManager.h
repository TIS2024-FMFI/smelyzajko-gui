#ifndef SHORTCUTS_MANAGER_H
#define SHORTCUTS_MANAGER_H

#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <chrono>
#include <GLFW/glfw3.h>

class ShortcutsManager {
public:
    using ShortcutCallback = std::function<void()>;

    explicit ShortcutsManager(GLFWwindow* window) : window(window) {}

    ShortcutsManager();

    void registerShortcut(const std::string& shortcut, ShortcutCallback callback);
    void processShortcuts();
    void setWindow(GLFWwindow* window);

private:
    std::unordered_map<std::string, ShortcutCallback> shortcuts;
    GLFWwindow* window;

    bool isShortcutPressed(const std::string& shortcut);

    int getGlfwKey(const std::string &key);

    std::vector<std::string> parseShortcut(const std::string &shortcut);

    std::unordered_map<std::string, std::chrono::steady_clock::time_point> lastActivationTimes;

    static const int DEBOUNCE_MS = 200; // Debounce delay in milliseconds
};

#endif // SHORTCUTS_MANAGER_H
