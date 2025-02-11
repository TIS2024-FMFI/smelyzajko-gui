# ModuleManager API

This document provides an overview of the `ModuleManager` API, which is a singleton class responsible for managing the registration, logging, and updating of modules and graphic modules in the system. The `ModuleManager` class offers various methods to interact with these modules, including setting values, logging settings, and managing the active module.

# Module vs GraphicModule

## Module
- **`Module`** is a class that contains the logic of the module.
- It can be a sensor, actuator, or any other generative function.
- It can be registered with the `ModuleManager` using the `registerModule` method.
- It can use multiple `GraphicModule` classes to display its data, which must be set in `PossibleGraphicsElements`.
- It can set `PossibleInputElements` such as sliders, buttons, etc., to control the module.
- All **Modules** must inherit from the `Module` class.
### Registering Modules
To register a module, you can use the `registerModule` method. This method takes a module name and a pointer to the module instance. And returns the module ID.

```cpp
manager.registerModule("MyModule", &myModule);
```

### Updating Module Values
The `ModuleManager` class provides an `updateValueOfModule` witch can be called from your `Module` to update value of `GraphicModule`.

```cpp
manager.updateValueOfModule("MyModuleID", "MyGraphicModuleID", "value");
```

## GraphicModule
- **`GraphicModule`** is a class that contains the graphical representation of the module.
- It can be registered with the `ModuleManager` using the `registerGraphicModule` method.
- It can be used to display the data of a module.
- The value of the `GraphicModule` can be updated by calling the `updateValueOfModule` method from the `Module`.
- It can also be used to log and replay the runs of the module.
- All **GraphicModules** must inherit from the `GraphicModule` class.


### Creating New `GraphicModules`

If you want to create new `GraphicModules`, follow these steps:

1. **Inherit from `GraphicModule`:** Create a new class that inherits from the `GraphicModule` class.

2. **Add Constructor to `ModuleManager`:** Once the new `GraphicModules` class is created, add its constructor to the `moduleConstructors` map in the `ModuleManager.h` file. This map defines the available `GraphicModules` that the `ModuleManager` can instantiate.

Here is an example of how to register a new `GraphicModules`:

```cpp
std::unordered_map<std::string, std::function<GraphicModule*()>> moduleConstructors = {
    {"MapGraphicElement", []() { return new MapModuleGraphics(); }},
    {"CounterGraphicElement", []() { return new CounterModuleGraphics(); }},
    {"UltrasonicGraphicElement", []() { return new UltrasonicModuleGraphics(); }},
    {"TextArea", []() { return new TextArea(); }},
    
    // Register your new graphic module here
    {"MyNewGraphicModule", []() { return new MyNewGraphicModule(); }},
};
````
### Example: Creating a New Graphic Module

####	1.	Define the New Graphic Module Class:
```cpp
class MyNewGraphicModule : public GraphicModule {
public:
    MyNewGraphicModule() {
        // Constructor implementation
    }
    
    void draw() override {
        // Custom drawing logic for the new graphic module
    }
};
```

####	2.	Add the Constructor to ModuleManager:
In the `ModuleManager.h` file, add an entry for your new graphic module in the `moduleConstructors` map:


### Registering Graphic Modules 
To register a graphic module, you can use the `registerGraphicModule` method. This method takes a graphic module name, the module name, and the module ID. And returns the graphic module ID.

```cpp
manager.registerGraphicModule("MyGraphicModule", "MyModule", MyModuleID);
```








