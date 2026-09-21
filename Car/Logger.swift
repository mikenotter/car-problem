import OSLog

func log(_ msg: String) {
    Log.instance.log(msg: msg)
}

class Log {
    let logger = Logger(subsystem: "com.me.car", category: "default")
    static let instance = Log();
    
    func log(msg: String) {
        logger.info("\(msg)")
    }
}
