#ifndef MAZE_FPSCOUNTER_H
#define MAZE_FPSCOUNTER_H

#include <chrono>
#include <iostream>

class FPSCounter {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> past;
    std::chrono::time_point<std::chrono::high_resolution_clock> future;
    int frameLimit;
    int now;
    double passed;
    double *durations;
public:
    explicit FPSCounter(int frameLimit);

    ~FPSCounter();

    double getFPS();

    void setFrameLimit(int newFrameLimit);

    void countFPSAndSleep();

    float getDelta();
};

#endif //MAZE_FPSCOUNTER_H
