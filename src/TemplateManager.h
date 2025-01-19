#pragma once
#include "Template.h"
#include "widgets/Element.h"
#include "Module.h"

class TemplateManager {
private:
    Template activeTemplate;
    bool configMode;
public:
    TemplateManager();
    TemplateManager(bool configMode);
    TemplateManager(const std::vector<std::string>& templateNames,bool configMode);

    std::vector<Template> allTemplates;
    void loadAllTemplates();
    void loadTemplates(const std::vector<std::string>& templateNames);
    void setActiveTemplate(Template aTemplate);
    void saveTemplate();
    void addModuleToActiveTemplate(GraphicModule *module);
    void saveCurrentTemplate(const std::string& fileName);
    void addElementToActiveTemplate(Element* element);
    void removeElementFromActiveTemplate(int index);
    void clearActiveTemplateElements();
    std::string getActiveTemplateName() const;
    std::vector<Element *>& getActiveTemplateElements();
    std::vector<GraphicModule *> getActiveTemplateModules();
    std::vector<Template> getAllTemplates();
    Template getActiveTemplate();
    void setConfigMode(bool mode);
};


