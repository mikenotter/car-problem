import Foundation
import SwiftUI
import QuartzCore

// Wraps the c++ controllers for driving the SwiftUI
// Also drives per-frame update triggers on the c++
@Observable
class SwiftWrapper {
    static var instance: SwiftWrapper? = nil
    private var autoPilot: UnsafeMutablePointer<AutoPilot>
    private var displayLink: DisplayLinkManager? = nil
    var x: Float = 0
    var spam: Bool = false
    var numClearedUpdates = 0
    
    var staticObjectsArray: Array<StaticObject> = []
    var dynamicObjectsArray: Array<DynamicObject> = []
    
    init() {
        autoPilot = UnsafeMutablePointer<AutoPilot>.allocate(capacity: 1)
        autoPilot.initialize(to: AutoPilot())
        autoPilot.pointee.setCallback(globalUpdateCallback)
        displayLink = .init(autoPilot: autoPilot, wrapper: self)
        
        Self.instance = self
    }
    
    func onUpdated() {
        let staticObjsVector = autoPilot.pointee.uiController.staticObjects
        let dynamicObjsVector = autoPilot.pointee.uiController.dynamicObjects
        staticObjectsArray = Array(staticObjsVector)
        dynamicObjectsArray = Array(dynamicObjsVector)
    }
    
    func printFirst() {
        if let first = staticObjectsArray.first {
            log("x: \(first.x), y: \(first.y)")
        }
    }
    
    func printArrays() {
        log("")
        log("")
        log("")
        for obj in staticObjectsArray {
            log("static, type: \(obj.type)")
        }
        for obj in dynamicObjectsArray {
            log("dynamic, type: \(obj.type)")
        }
    }
}

func globalUpdateCallback() {
    SwiftWrapper.instance?.onUpdated()
}

@Observable
class DisplayLinkManager {
    private var displayLink: CADisplayLink?
    private var autoPilot: UnsafeMutablePointer<AutoPilot>
    private let wrapper: SwiftWrapper
    private var lerpedX: Float
    
    init(autoPilot: UnsafeMutablePointer<AutoPilot>, wrapper: SwiftWrapper) {
        self.autoPilot = autoPilot
        self.wrapper = wrapper
        self.lerpedX = wrapper.x
        start()
    }

    func start() {
        guard let screen = NSScreen.main else { return }
        displayLink = screen.displayLink(target: self, selector: #selector(tick))
        displayLink?.add(to: .main, forMode: .common)
    }

    func stop() {
        displayLink?.invalidate()
        displayLink = nil
    }

    @objc func tick(displayLink: CADisplayLink) {
        let deltaTime = displayLink.targetTimestamp - displayLink.timestamp
        lerpedX = lerpedX + (wrapper.x - lerpedX) * Float(deltaTime) * 2
        
        // send in current state with the update
        autoPilot.pointee.update(deltaTime, lerpedX, wrapper.spam)
        wrapper.spam = false
    }
}

extension ObjectType: CustomStringConvertible {
    public var description: String {
        switch self {
        case ObjectType.Person:
            return "Person"
        case ObjectType.Animal:
            return "Animal"
        case ObjectType.Car:
            return "Car"
        case ObjectType.StopSign:
            return "StopSign"
        case ObjectType.SpeedBump:
            return "SpeedBump"
        case ObjectType.Tree:
            return "Tree"
        default:
            return "Unknown"
        }
    }
}

extension ObjectType {
    public var iconName: String {
        switch self {
        case ObjectType.Person:
            return "figure.walk.suitcase.rolling"
        case ObjectType.Animal:
            return "hare"
        case ObjectType.Car:
            return "car"
        case ObjectType.StopSign:
            return "xmark.circle"
        case ObjectType.SpeedBump:
            return "square"
        case ObjectType.Tree:
            return "tree"
        default:
            return "Unknown"
        }
    }
}

extension StaticObject: Identifiable {
    public var id: Int32 {
        return objectId;
    }
}

extension DynamicObject: Identifiable {
    public var id: Int32 {
        return objectId;
    }
}

