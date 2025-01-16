#ifndef MODULE_H
#define MODULE_H
#include "imgui.h"
#include <string>
#include "libs/json.hpp"
class Module {
public:
    ~Module() = default;

    virtual void run() = 0;
    virtual std::vector<std::string>  getPossibleGraphicsElement() = 0;


    // Module settings
    int getModuleID() const ;
    void setModuleID(int id) ;
    std::string getModuleName() const;
    void setModuleName(std::string name) ;

    // Module settings
    int moduleId;
    std::string moduleName;
};

#endif // MODULE_H
