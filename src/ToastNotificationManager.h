#pragma once

#include <imgui.h>
#include <vector>
#include <string>
#include <chrono>


/**
 * @class GraphicModule
 * @brief GraphicModule class is an abstract class that represents a graphic element in the GUI.
 */




class ToastNotificationManager {
public:
    struct Notification {
        std::string title;
        std::string message;
        std::chrono::time_point<std::chrono::steady_clock> start_time;
        bool is_open = false; // Tracks if the notification is still open
    };

    void addNotification(const std::string& title = "", const std::string& message = "");
    void renderNotifications();

private:
    std::vector<Notification> notifications;
    const int timeout = 5;
};

