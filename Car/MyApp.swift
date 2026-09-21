import SwiftUI

@main struct MyApp: App {
    var wrapper = SwiftWrapper()
    var body: some Scene {
        WindowGroup {
            ContentView(wrapper: wrapper)
        }
    }
}
