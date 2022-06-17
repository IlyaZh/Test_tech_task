#include <iostream>
#include <chrono>
#include <string>

class Timer {
    std::string name;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
public:
    Timer(std::string& timerName) :
    name(timerName),
    start(std::chrono::high_resolution_clock::now())
    {}
    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Timer \"" << name << "\" " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms" <<  std::endl;
    }
};