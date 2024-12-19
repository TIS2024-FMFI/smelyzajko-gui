#pragma once
#include "Template.h"
#include "Module.h"
class TemplateManager {
private:
    Template activeTemplate;
    std::vector<Template> allTemplates;
    void loadAllTemplates();
public:
    TemplateManager();
    void setActiveTemplate(Template aTemplate);
    void addModuleToActiveTemplate(GraphicModule *module);
    void saveCurrentTemplate(const std::string& fileName);
    void addElementToActiveTemplate(Element* element);
    void removeElementFromActiveTemplate(int index);
    void clearActiveTemplateElements();
    std::string getActiveTemplateName() const;
    std::vector<Element *> getActiveTemplateElements();
    std::vector<GraphicModule *> getActiveTemplateModules();
    std::vector<Template> getAllTemplates();

};


