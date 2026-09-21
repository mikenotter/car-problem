#include "CppTypes.hpp"
#include <thread>

UIController::UIController() {
    //worker = std::thread(&UIController::popExtraUpdates, this);
}

void UIController::receive(UpdateInfo updateInfo) {
    updateQueue.push(updateInfo);
}

void UIController::processUpdate() {
    //std::lock_guard<std::mutex> lock(mutex);
    if (!updateQueue.empty()) {
        auto update = std::move(updateQueue.front());
        updateQueue.pop();
        
        char* buf = update.buf.get();
        auto staticCount = update.staticCount;
        auto dynamicCount = update.dynamicCount;
        
        int staticObjectSize = StaticObject::getSize();
        int dynamicObjectSize = DynamicObject::getSize();
        
        // fill static objects
        std::vector<StaticObject> newStaticObjects;
        
        for (int i = 0; i < staticCount; i++) {
            StaticObject staticObject;
            std::memcpy(&staticObject, buf, staticObjectSize);
            newStaticObjects.push_back(staticObject);
            
            buf += staticObjectSize;
        }
        
        // fill dynamic objects
        std::vector<DynamicObject> newDynamicObjects;
        
        for (int i = 0; i < dynamicCount; i++) {
            DynamicObject dynamicObject;
            std::memcpy(&dynamicObject, buf, dynamicObjectSize);
            newDynamicObjects.push_back(dynamicObject);
            
            buf += dynamicObjectSize;
        }
        
        this->staticObjects = std::move(newStaticObjects);
        this->dynamicObjects = std::move(newDynamicObjects);
        
        // update swiftUI
        if (onUpdated) {
            onUpdated();
        }
        
//        if (!updateQueue.empty()) {
//            needsFlush = true;
//            cv.notify_one();
//        }
    }
}

void UIController::setCallback(VoidCallback onUpdated) {
    this->onUpdated = std::move(onUpdated);
}

// for now, worker thread just pops extra messages - should also check for "important" updates
//void UIController::popExtraUpdates() {
//    while (true) {
//        {
//            std::unique_lock<std::mutex> lock(mutex);
//            cv.wait(lock, [this]() {
//                return needsFlush && updateQueue.size() > 1;
//            });
//
//            if (updateQueue.size() > 1) {
//                updateQueue.pop();
//                extraUpdatesPopped++;
//            } else {
//                needsFlush = false;
//            }
//        }
//    }
//}
