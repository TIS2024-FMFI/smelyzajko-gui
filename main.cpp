#include "src/ConfigurationMode.h"
#include "src/OperatingMode.h"

int main() {
bool config = false;

if (config){
    ConfigurationMode gui;
    gui.run();
}
else{
    OperatingMode operatingMode;
    operatingMode.run();
}


}

