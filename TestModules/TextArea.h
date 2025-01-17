#ifndef SMELYZAJKO_GUI_TEXTAREA_H
#define SMELYZAJKO_GUI_TEXTAREA_H

#include "imgui.h"
#include "Scrollbar.h"
#include <vector>
#include <string>
#include <mutex>

class TextArea {
public:
    TextArea(float width, float height, const std::string& id); // Constructor

    // Set a new width dynamically
    void setWidth(float newWidth);

    // Draw the text area and handle interactions
    void drawTextArea(ImVec2 position, ImGuiIO& io);

    // Manage logs
    void addLog(const std::string& log);
    void clearLogs();

    // Autoscroll control
    void setAutoscrollEnabled(bool enabled);
    bool isAutoscrollEnabled() const;

private:
    float width;                      // Width of the text area
    float height;                     // Height of the text area
    std::string id;                   // Unique identifier for the text area
    Scrollbar scrollbar;              // Scrollbar instance
    std::vector<std::string> logs;    // Logs displayed in the text area
    bool autoscrollEnabled;           // Autoscroll toggle
    std::mutex logMutex;              // Mutex for thread-safe log updates
};

#endif // SMELYZAJKO_GUI_TEXTAREA_H
