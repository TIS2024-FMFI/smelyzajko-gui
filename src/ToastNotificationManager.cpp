#include "ToastNotificationManager.h"

void ToastNotificationManager::addNotification(const std::string& title, const std::string &message) {
    notifications.push_back({title, message, std::chrono::steady_clock::now(), true});
}


void ToastNotificationManager::renderNotifications() {
    if (notifications.empty()) return;

    ImGuiIO& io = ImGui::GetIO();
    float padding = 10.0f;
    float y_offset = 0.0f;

    for (size_t i = 0; i < notifications.size();) {
        auto& notification = notifications[i];
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - notification.start_time
        ).count();

        if (!notification.is_open || elapsed >= timeout) {
            notifications.erase(notifications.begin() + i);
            continue;
        }

        ImVec2 window_pos = ImVec2(io.DisplaySize.x - padding, io.DisplaySize.y - padding - y_offset);
        ImVec2 window_pos_pivot = ImVec2(1.0f, 1.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowBgAlpha(0.8f);

        // Use both the title and an index to ensure uniqueness in the window label
        std::string windowLabel = notification.title.empty() ? "ToastNotification" : notification.title;
        windowLabel += "##" + std::to_string(i); // Add unique index for non-unique titles

        if (ImGui::Begin(windowLabel.c_str(), &notification.is_open,
                         ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoFocusOnAppearing |
                         ImGuiWindowFlags_NoNav |
                         ImGuiWindowFlags_NoCollapse)) {

            ImGui::TextUnformatted(notification.message.c_str());
            y_offset += ImGui::GetWindowHeight() + 5.0f; // Move the next notification up
        }
        ImGui::End();

        ++i;
    }
}
