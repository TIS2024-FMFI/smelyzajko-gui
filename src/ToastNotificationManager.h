#include <imgui.h>
#include <vector>
#include <string>
#include <chrono>

class ToastNotificationManager {
public:
    struct Notification {
        std::string message;
        std::chrono::time_point<std::chrono::steady_clock> start_time;
        bool is_open = false; // Tracks if the notification is still open
    };

    void addNotification(const std::string& message);
    void renderNotifications();

private:
    std::vector<Notification> notifications;
    const int timeout = 10;
};

