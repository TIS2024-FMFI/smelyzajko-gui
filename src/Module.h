// File: Module.h
#ifndef MODULE_H
#define MODULE_H

#include "imgui.h"
#include <string>

class Module {
public:
    virtual ~Module() = default;
    virtual void renderStandalone() = 0;
    virtual ImVec2 getSize() = 0;
    virtual ImVec2 getPos() = 0;
    virtual std::string getName() const = 0;
    virtual void setPos(ImVec2 pos) = 0;
    virtual void setSize(ImVec2 size)  = 0;
    int counter = 1;

private:
    std::string name;
    ImVec2 pos;
    ImVec2 size;
};

#endif // MODULE_H