#ifndef MODULE_H
#define MODULE_H

#include "imgui.h"
#include <string>
#include "json.hpp"
class Module {
public:
    ~Module() = default;
    Module() : graphicsFrequency(10.0f), graphicsLogEnabled(false),
                       textFrequency(10.0f), textLogEnabled(false) {}

    Module(int id, const char* name);

    virtual void run() = 0;



    virtual std::string getName() const = 0;
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

    // Graphics settings
    float graphicsFrequency;
    bool graphicsLogEnabled;

    // Text settings
    float textFrequency;
    bool textLogEnabled;

};
#endif // MODULE_H
