#ifndef COUNTERMODULE_H
#define COUNTERMODULE_H

#include <vector>
#include "imgui.h"
#include "../src/Module.h"

class CounterModule : public Module {
public:
    void renderStandalone() override;

    void saveLogToJson(const std::vector<int> &values);
    std::string getName() const override;

    ImVec2 getSize() override;

    ImVec2 getPos() override;

    void setPos(ImVec2 pos) override;

    void setSize(ImVec2 size) override;

private:
    std::string name = "Counter Module";
    ImVec2 size;
    ImVec2 pos;
};


#endif // COUNTERMODULE_H
