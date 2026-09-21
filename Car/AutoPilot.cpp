#include "CppTypes.hpp"
#include <random>

AutoPilot::AutoPilot() {
    setupObjects();
}

void AutoPilot::update(double deltaTime, float x, bool spam) {
    updateObjectsAndSend(x,spam);
    
    uiController.processUpdate();
}

void AutoPilot::updateObjectsAndSend(float x, bool spam) {
    for (auto& obj : staticObjects) {
        auto y = obj.y + 0.001;
        if (y > 0.5) y = -0.5;
        obj.y = y;
        
        obj.x = obj.initialX - x;
    }
    for (auto& obj : dynamicObjects) {
        auto y = obj.y + 0.001;
        if (y > 0.5) y = -0.5;
        obj.y = y;
        
        obj.x = obj.initialX - x;
    }
    send(spam);
}

void AutoPilot::send(bool spam) {
    auto numStatic = staticObjects.size();
    auto numDynamic = dynamicObjects.size();
    auto staticBytesSize = numStatic * StaticObject::getSize();
    auto dynamicBytesSize = numDynamic * DynamicObject::getSize();
    std::shared_ptr<char[]> buf = std::make_shared<char[]>(staticBytesSize + dynamicBytesSize);
    
    std::memcpy(buf.get(), staticObjects.data(), staticBytesSize);
    std::memcpy(buf.get() + staticBytesSize, dynamicObjects.data(), dynamicBytesSize);
    
    uiController.receive({buf,numStatic,numDynamic});
    
    if (spam) {
        for (int i = 0; i < 20; i++) {
            std::shared_ptr<char[]> buf = std::make_shared<char[]>(staticBytesSize + dynamicBytesSize);
            
            std::memcpy(buf.get(), staticObjects.data(), staticBytesSize);
            std::memcpy(buf.get() + staticBytesSize, dynamicObjects.data(), dynamicBytesSize);
            
            uiController.receive({buf,numStatic,numDynamic});
        }
    }
}

void AutoPilot::setCallback(VoidCallback onUpdated) {
    uiController.setCallback(onUpdated);
}

void AutoPilot::setupObjects() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distrib(0, 1.0f);
    
    for (int i = 0; i < 10; i++) {
        auto type = i % 3 == 0 ? ObjectType::StopSign : i % 3 == 1 ? ObjectType::SpeedBump : ObjectType::Tree;
        float x = distrib(gen) - 0.5;
        float y = float(i) / 10.0 - 0.5;
        staticObjects.push_back(StaticObject(type,i,x,y));
    }
    
    for (int i = 0; i < 10; i++) {
        auto type = i % 3 == 0 ? ObjectType::Person : i % 3 == 1 ? ObjectType::Animal : ObjectType::Car;
        float x = distrib(gen) - 0.5;
        float y = float(i) / 10.0 - 0.5;
        dynamicObjects.push_back(DynamicObject(type,i+100,x,y));
    }
}

StaticObject::StaticObject() {}
DynamicObject::DynamicObject() {}
