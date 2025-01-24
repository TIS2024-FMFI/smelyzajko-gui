#ifndef SMELYZAJKO_GUI_TEXTAREA_H
#define SMELYZAJKO_GUI_TEXTAREA_H

#include "imgui.h"
#include "../src/Module.h"
#include "../src/GraphicModule.h"
#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include "Scrollbar.h"
#include <atomic>



class TextArea : public GraphicModule  {
public:
    TextArea();

    // Draw the text area and handle interactions
    void draw (ImGuiIO& io) override;
    void logToJson() override;
    void logFromJson() override;
    void logForward() override;
    void logBackwards() override;
    void updateValueOfModule(std::string value ) override;
    void startLoggingThread() override;

    void clearLogs();

    // Autoscroll control
    void setAutoscrollEnabled(bool enabled);
    bool isAutoscrollEnabled() const;





private:
    void stopLoggingThread();
    void loggingThreadFunction();
    std::thread loggingThread;
    std::atomic<bool> loggingThreadRunning;
    Scrollbar scrollbar;              // Scrollbar instance
    std::vector<std::string> logs;    // Logs displayed in the text area
    bool autoscrollEnabled = true;           // Autoscroll toggle
    std::mutex logMutex;              // Mutex for thread-safe log updates

};

#endif // SMELYZAJKO_GUI_TEXTAREA_H