#ifndef MODULE_H
#define MODULE_H
#include "imgui.h"
#include <string>
#include "libs/json.hpp"
#include "ShortcutsManager.h"
#include "ToastNotificationManager.h"

class Module {
public:
    ~Module() = default;

    virtual void run() = 0;
    virtual std::vector<std::string>  getPossibleGraphicsElement() = 0;
    virtual std::unordered_map<std::string,std::vector<std::string>> getPossibleInputElements() = 0;

    virtual void setValueFromInputElements(std::string elementName, std::string value);
    virtual void setValueFromInputElements(std::string elementName, int value);
    virtual void setValueFromInputElements(std::string elementName, float value);
    virtual void setValueFromInputElements(std::string elementName, bool value);
    virtual void setValueFromInputElements(std::string elementName, std::vector<int> value);

    virtual void registerShortcuts(ShortcutsManager& shortcutsManager, ToastNotificationManager& toastNotificationManager) = 0;

    // Module settings
    int getModuleID() const ;
    void setModuleID(int id) ;
    std::string getModuleName() const;
    void setModuleName(std::string name) ;

    int moduleId;
    std::string moduleName;
};

#endif // MODULE_H
