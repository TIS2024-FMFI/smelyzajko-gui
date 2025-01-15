#include "GraphicModule.h"

void GraphicModule::scaleFromResolution(ImVec2 templateResolution) {
    ImVec2 currentResolution = ImGui::GetIO().DisplaySize;

    // Calculate scale factors
    float scaleX = currentResolution.x / templateResolution.x;
    float scaleY = currentResolution.y / templateResolution.y;

    position.x *= scaleX;
    position.y *= scaleY;

    size.x *= scaleX;
    size.y *= scaleY;
}