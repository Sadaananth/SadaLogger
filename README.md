# SadaLogger

This is just another lightweight C++ logger implementation

# How to use
This logger is simple to use. Link the library `libSadaLogger.so` and include the header file `Logger.hpp`.
Logger is implemented as simpleton, So only one instance is available throughout the lifetime providing easy use

After adding to the build system. To print,

* Add a sink in the `main.cpp` file or any of the `init` funtion. It can any sink Console, File or any GuiSink.
    - `Logger::instance().add_sink(Logger::Sink::console); `
* Once sink is added, we can log whatever we want now
    - `LOG_DEBUG << "This is a debug message";`
    - `LOG_INFO << "This is a info message";`
    - `LOG_WARN << "This is a warning message";`
    - `LOG_ERROR << "This is a error message";`
* And the formatted log will be
    - `[Debug] LoggerTest.cpp:11 This is a debug message`
    - `[Info] LoggerTest.cpp:12 This is a info message`
    - `[Warning] LoggerTest.cpp:13 This is a warning message`
    - `[Error] LoggerTest.cpp:14 This is a error message`

# How to build
To build the logger library, simply run the command `./build.sh` 

# Run unit tests
To run unit tests, run the command `./run_tests.sh`. Running this command will also build the library.
