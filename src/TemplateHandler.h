#pragma once
#include "Template.h"

class TemplateHandler {
public:
    TemplateHandler();
    Template activeTemplate;
    std::vector<Template> allTemplates;

    void loadAllTemplates();
    void setActiveTemplate(Template aTemplate);
};


