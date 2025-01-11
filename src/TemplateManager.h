#include "Template.h"

class TemplateManager {
private:
    Template activeTemplate;
public:
    TemplateManager();
    std::vector<Template> allTemplates;
    void loadAllTemplates();
    void setActiveTemplate(Template aTemplate);
    void saveTemplate();
    void addElementToActiveTemplate(Element* element);
    void removeElementFromActiveTemplate(int index);
    void clearActiveTemplateElements();
    std::string getActiveTemplateName() const;
    std::vector<Element *> getActiveTemplateElements();
    std::vector<Template> getAllTemplates();
    Template getActiveTemplate();
};


