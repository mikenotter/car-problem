import SwiftUI
import Playgrounds

struct ContentView: View {
    let wrapper: SwiftWrapper
    let size: CGFloat = 500
    var body: some View {
        HStack {
            ZStack {
                ForEach(wrapper.staticObjectsArray, content: { item in
                    Image(systemName: item.type.iconName)
                        .offset(x: CGFloat(item.x) * size, y: CGFloat(item.y) * size)
                })
                
                ForEach(wrapper.dynamicObjectsArray, content: { item in
                    Image(systemName: item.type.iconName)
                        .offset(x: CGFloat(item.x) * size, y: CGFloat(item.y) * size)
                })
                
                Image(systemName: "car.rear.fill")
                    .resizable()
                    .frame(width: size / 10, height: size / 10)
            }
            .frame(width: size, height: size)
            .clipShape(RoundedRectangle(cornerRadius: 100))
            .overlay(
                RoundedRectangle(cornerRadius: 100)
                    .strokeBorder(Color.yellow, lineWidth: 4)
            )
            
            VStack {
                HStack {
                    Button(action: {
                        wrapper.x -= 0.01
                    }) {
                        ZStack {
                            Image(systemName: "arrowshape.left")
                                .resizable()
                                .frame(width: 40, height: 40)
                            RoundedRectangle(cornerRadius: 10)
                                .strokeBorder(.yellow, lineWidth: 4)
                                .frame(width: 100, height: 60)
                            
                        }
                    }
                    Button(action: {
                        wrapper.x += 0.01
                    }) {
                        ZStack {
                            Image(systemName: "arrowshape.right")
                                .resizable()
                                .frame(width: 40, height: 40)
                            RoundedRectangle(cornerRadius: 10)
                                .strokeBorder(.yellow, lineWidth: 4)
                                .frame(width: 100, height: 60)
                            
                        }
                    }
                }
                
                Button(action: {
                    wrapper.spam = true
                }) {
                    ZStack {
                        Text("Spam")
                            .font(.largeTitle)
                            .frame(width: 120, height: 100)
                        RoundedRectangle(cornerRadius: 10)
                            .strokeBorder(.yellow, lineWidth: 4)
                            .frame(width: 200, height: 120)
                        
                    }
                }
            }
        }
    }
}
