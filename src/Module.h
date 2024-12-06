
#ifndef MODULE_H
#define MODULE_H

#include "imgui.h"

#include <string>

class Module {
public:
    virtual ~Module() = default;
    Module() : graphicsFrequency(10.0f), graphicsLogEnabled(false),
                       textFrequency(10.0f), textLogEnabled(false) {}
    virtual void renderStandalone(ImGuiIO io, ImVec2 possition) = 0;
    virtual ImVec2 getSize() = 0;
    virtual ImVec2 getPos() = 0;
    virtual std::string getName() const = 0;
    virtual void setPos(ImVec2 pos) = 0;
    virtual void setSize(ImVec2 size)  = 0;
    virtual void draw(ImGuiIO &io) = 0 ;
    int counter = 0;
    bool active = false;
    Module(int id, const char* name);

    // Graphics Settings
    float GetGraphicsFrequency() const;
    void SetGraphicsFrequency(float freq);

    bool IsGraphicsLoggingEnabled() const;
    void SetGraphicsLoggingEnabled(bool enabled);

    // Text Settings
    float GetTextFrequency() const;
    void SetTextFrequency(float freq);

    bool IsTextLoggingEnabled() const;
    void SetTextLoggingEnabled(bool enabled);

    int moduleId;            // Module ID for identification
    std::string moduleName;  // Module name


private:
    std::string name;
    ImVec2 possition;
    ImVec2 size;
    // Graphics settings
    float graphicsFrequency;
    bool graphicsLogEnabled;

    // Text settings
    float textFrequency;
    bool textLogEnabled;
};

#endif // MODULE_H