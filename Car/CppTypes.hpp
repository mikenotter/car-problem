#pragma once
#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>

using VoidCallback = void(*)();

enum class ObjectType : u_int8_t {
    Person, Animal, Car, StopSign, SpeedBump, Tree
};

struct UpdateInfo {
    std::shared_ptr<char[]> buf;
    unsigned long staticCount;
    unsigned long dynamicCount;
};

// static and dynamic objects are currently identical, but both are included to show combining bytes into a buffer
struct StaticObject {
public:
    ObjectType type;
    int objectId;
    float initialX;
    float x;
    float y;
    
    StaticObject();
    StaticObject(ObjectType type, int id, float x, float y) : type(type), objectId(id), initialX(x), x(x), y(y) {};
    static int getSize() { return sizeof(StaticObject); }
};

struct DynamicObject {
public:
    ObjectType type;
    int objectId;
    float initialX;
    float x;
    float y;
    
    DynamicObject();
    DynamicObject(ObjectType type, int id, float x, float y) : type(type), objectId(id), initialX(x), x(x), y(y) {};
    static int getSize() { return sizeof(DynamicObject); }
};

// The receiving end of the message transfer
class UIController {
private:
    std::queue<UpdateInfo> updateQueue;
    VoidCallback onUpdated;
    
    // I began adding threading as an example of handling message volume overflow - but Swift/C++ interop became problematic :(
    //std::mutex mtx;
    //std::condition_variable cv;
    //std::thread worker;
    //bool needsFlush;
    //void popExtraUpdates();
    //int extraUpdatesPopped;
public:
    UIController();
    std::vector<StaticObject> staticObjects;
    std::vector<DynamicObject> dynamicObjects;
    void setCallback(VoidCallback onUpdated);
    void receive(UpdateInfo updateInfo);
    void processUpdate();
    void printObjects();
};

// The sending end of message transfer - also updates the state of tracked objects
class AutoPilot {
private:
    std::vector<StaticObject> staticObjects;
    std::vector<DynamicObject> dynamicObjects;
    double elapsed;
    double lastUpdateTime;
public:
    AutoPilot();
    UIController uiController;
    void setCallback(VoidCallback onUpdated);
    void setupObjects();
    void update(double deltaTime, float carX, bool spam);
    void updateObjectsAndSend(float x, bool spam);
    void send(bool spam);
};

