### Quick exercise playing with converting a collection of objects to a buffer for network transmission.

https://github.com/user-attachments/assets/4e8e54af-2e0a-4136-ba7a-1d477a9ba6a6

#### Features:

##### C++
- static and dynamic objects are tracked and updated by a c++ driver
- object collections are converted to a byte buffer and sent to a ui controller each frame
- ui controller converts the buffer back to object collections for both object types
- ui controller triggers an update in the swiftUI for rendering

##### Swift
- wrapper class manages the c++ driver and triggers per-frame updates
- swiftUI renders a unique icon for each object type and places each object based on its location
- arrow keys push horizontal location updates into the c++ driver
- spam button floods the message queue, but overflow control is currently disabled due to swift/c++ interop issues for multithreading (see below)

#### Notes:
- I began adding multithreading to simulate message overflow control and processing
  - idea was to process excess messages between frame updates - eventually looking for "important" messages during processing
  - but swift/c++ interop got messy when I began adding the relevant properties, so it's been commented out
- some details like buffer sizing have been removed for expediency
