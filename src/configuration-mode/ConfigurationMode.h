#include "../GUI.h"
#include "ElementPlaceholder.h"

class ConfigurationMode : GUI {
public:
    std::vector<ElementPlaceholder> activeElementPlaceholders;
    int run() override;

    void drawElementPlaceholders();
    void setupMenuBar();
};


