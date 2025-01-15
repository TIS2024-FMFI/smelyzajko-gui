#include "Module.h"

Module::Module(int id, const char* name)
        : moduleId(id), moduleName(name), graphicsFrequency(10.0f), graphicsLogEnabled(false),
          textFrequency(10.0f), textLogEnabled(false) {}


float Module::GetGraphicsFrequency() const { return graphicsFrequency; }
void Module::SetGraphicsFrequency(float freq) { graphicsFrequency = freq; }

bool Module::IsGraphicsLoggingEnabled() const { return graphicsLogEnabled; }
void Module::SetGraphicsLoggingEnabled(bool enabled) { graphicsLogEnabled = enabled; }

float Module::GetTextFrequency() const { return textFrequency; }
void Module::SetTextFrequency(float freq) { textFrequency = freq; }

bool Module::IsTextLoggingEnabled() const { return textLogEnabled; }
void Module::SetTextLoggingEnabled(bool enabled) { textLogEnabled = enabled; }

