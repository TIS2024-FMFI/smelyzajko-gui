#ifndef MODULE_H
#define MODULE_H

#include <string>

class Module {
public:
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
    // Graphics settings
    float graphicsFrequency;
    bool graphicsLogEnabled;

    // Text settings
    float textFrequency;
    bool textLogEnabled;
};

#endif // MODULE_H
