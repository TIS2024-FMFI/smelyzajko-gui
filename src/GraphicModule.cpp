#include "GraphicModule.h"
#include "GLFW/glfw3.h"
void GraphicModule::scaleFromResolution(ImVec2 templateResolution) {
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

    int monitorWidth = videoMode->width;
    int monitorHeight = videoMode->height;

    float scaleX = monitorWidth / templateResolution.x;
    float scaleY = monitorHeight / templateResolution.y;

    position.x *= scaleX;
    position.y *= scaleY;

    size.x *= scaleX;
    size.y *= scaleY;
}

// Modules settings
void GraphicModule::setModuleName(std::string name) {moduleName = name;}
std::string GraphicModule::getModuleName() const {return moduleName;}
int GraphicModule::getModuleID() const {return moduleId;}
void GraphicModule::setModuleID(int id) {moduleId = id;}

// Graphic module element settings
void GraphicModule::setGraphicElementName(std::string name) {graphicElementName = name;}
std::string GraphicModule::getGraphicElementName() const {return graphicElementName;}
int GraphicModule::getGraphicModuleID() const {return graphicElementId;}
void GraphicModule::setGraphicModuleID(int id) {graphicElementId = id;}

//Update value of module
void GraphicModule::updateValueOfModule(std::string value) {
    std::cerr << "Not Implemented" << std::endl;
}
void GraphicModule::updateValueOfModule(int value) {
    std::cerr << "Not Implemented" << std::endl;
}
void GraphicModule::updateValueOfModule(std::vector<int> value) {
    std::cerr << "Not Implemented" << std::endl;
}


// Position and size settings
ImVec2 GraphicModule::getSize() {return size;}
ImVec2 GraphicModule::getPos() {return position;}
void GraphicModule::setPos(ImVec2 pos_) {position = pos_;}
void GraphicModule::setSize(ImVec2 size_) {size = size_;}


//Saving and loading to/from template
void GraphicModule::to_json(nlohmann::json& j) const {
    j["graphicElementId"] = graphicElementId;
    j["graphicElementName"] = graphicElementName;
    j["moduleId"] = moduleId;
    j["moduleName"] = moduleName;
    j["position"] = {position.x, position.y};
    j["size"] = {size.x, size.y};
    j["graphicsFrequency"] = graphicsFrequency;
    j["graphicsLogEnabled"] = graphicsLogEnabled;
    j["textFrequency"] = textFrequency;
    j["textLogEnabled"] = textLogEnabled;
}
void GraphicModule::from_json(const nlohmann::json& j, ImVec2 resolution) {
    if (j.contains("graphicElementId") && j["graphicElementId"].is_number_integer()) {
        graphicElementId = j["graphicElementId"];
    }
    if (j.contains("graphicElementName") && j["graphicElementName"].is_string()) {
        graphicElementName = j["graphicElementName"];
    }
    if (j.contains("moduleId") && j["moduleId"].is_number_integer()) {
        moduleId = j["moduleId"];
    }
    if (j.contains("moduleName") && j["moduleName"].is_string()) {
        moduleName = j["moduleName"];
    }
    if (j.contains("position") && j["position"].is_array() && j["position"].size() == 2) {
        position.x = j["position"][0];
        position.y = j["position"][1];
        setPos({j["position"][0], j["position"][1]});
    }
    if (j.contains("size") && j["size"].is_array() && j["size"].size() == 2) {
        size.x = j["size"][0];
        size.y = j["size"][1];
        setSize({j["size"][0], j["size"][1]});
    }
    if (j.contains("graphicsFrequency") && j["graphicsFrequency"].is_number_float()) {
        graphicsFrequency = j["graphicsFrequency"];
    }
    if (j.contains("graphicsLogEnabled") && j["graphicsLogEnabled"].is_boolean()) {
        graphicsLogEnabled = j["graphicsLogEnabled"];
    }
    if (j.contains("textFrequency") && j["textFrequency"].is_number_float()) {
        textFrequency = j["textFrequency"];
    }
    if (j.contains("textLogEnabled") && j["textLogEnabled"].is_boolean()) {
        textLogEnabled = j["textLogEnabled"];
    }
    scaleFromResolution(resolution);
}