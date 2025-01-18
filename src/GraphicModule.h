#ifndef SMELYZAJKO_GUI_GRAPHICMODULE_H
#define SMELYZAJKO_GUI_GRAPHICMODULE_H
#include "iostream"
#include "imgui.h"
#include "libs/json.hpp"


class GraphicModule {
public:
    virtual void draw(ImGuiIO &io) = 0;

    virtual void logToJson() = 0;

    // Position and size settings
    ImVec2 getSize() ;
    ImVec2 getPos() ;
    virtual void setPos(ImVec2 pos_);
    virtual void setSize(ImVec2 size_);

    // Update value of module
    virtual void updateValueOfModule(std::string value) ;
    virtual void updateValueOfModule(int value) ;
    virtual void updateValueOfModule(std::vector<int> value) ;
    virtual void updateValueOfModule(std::vector<float> value) ;

    // Modules settings
    std::string getModuleName() const ;
    void setModuleName(std::string name) ;
    int getModuleID() const;
    void setModuleID(int id);

    // Graphic module element settings
    std::string getGraphicElementName() const ;
    void setGraphicElementName(std::string name);
    int getGraphicModuleID() const;
    void setGraphicModuleID(int id) ;

    //Saving and loading to/from template
    virtual void to_json(nlohmann::json& j) const ;
    virtual void from_json(const nlohmann::json& j, ImVec2 resolution);
    virtual void scaleFromResolution(ImVec2 templateResolution);

    //frequency settings and log settings
    float getGraphicsFrequency() const;
    void setGraphicsFrequency(float frequency);

    bool isGraphicsLogEnabled() const;
    void setGraphicsLogEnabled(bool logEnabled);

    // Text frequency settings
    float getTextFrequency() const;
    void setTextFrequency(float frequency);

    bool isTextLogEnabled() const;
    void setTextLogEnabled(bool logEnabled);

    void setLogFileName(std::string name);
    std::string getLogFileName() const;
protected:
    // Graphic module element settings
    int graphicElementId;
    std::string graphicElementName;

    // Module settings
    int moduleId;
    std::string moduleName;

    // Position and size settings
    ImVec2 position;
    ImVec2 size;

    // Graphics settings
    float graphicsFrequency;
    bool graphicsLogEnabled;

    // Text settings
    float textFrequency;
    bool textLogEnabled;

    std::string logFileDirectory;

};


#endif //SMELYZAJKO_GUI_GRAPHICMODULE_H
