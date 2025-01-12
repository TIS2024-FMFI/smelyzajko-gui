#include "GUI.h"
#include "TemplateManager.h"
#include "ToastNotificationManager.h"
#include <algorithm>
#include "Module.h"
#include "ImGuiFileDialog/ImGuiFileDialog.h"

class ConfigurationMode : GUI {
public:
    ConfigurationMode() : io(ImGui::GetIO()) {}

    ToastNotificationManager toastManager;
    TemplateManager templateManager;
    ImGuiIO& io;

    int run() override;

    void addElementToActiveTemplate(Element* element);

    void drawElements();
    void setupMenuBar();
    void drawGrid() const;
    void bringElementToTop(Element* element);
    void renderSettingsPopup(Module& module, const std::string& part);

    void createFloatSliderSettings();
    void createIntSliderSettings();
    void createLabelSettings();
private:
    float gridSize = 60.0f;
    bool isSnapping = false;
    bool showGrid = false;
};


