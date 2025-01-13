#include "ToastNotificationManager.h"

void ToastNotificationManager::addNotification(const std::string &message) {
    notifications.push_back({message, std::chrono::steady_clock::now(), true});
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

        // Remove the notification if it is manually closed or after 'timeout' seconds
        if (!notification.is_open || elapsed >= timeout) {
            notifications.erase(notifications.begin() + i);
            continue;
        }

        // Position the notification window at the bottom-right corner
        ImVec2 window_pos = ImVec2(io.DisplaySize.x - padding, io.DisplaySize.y - padding - y_offset);
        ImVec2 window_pos_pivot = ImVec2(1.0f, 1.0f); // Bottom-right alignment
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowBgAlpha(0.8f); // Adjust transparency

        // render
        if (ImGui::Begin(("##ToastNotification" + std::to_string(i)).c_str(), &notification.is_open,
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
