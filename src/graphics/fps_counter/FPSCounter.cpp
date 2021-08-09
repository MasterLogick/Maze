#include <thread>
#include "FPSCounter.h"


FPSCounter::FPSCounter(int frameLimit) : frameLimit(frameLimit) {
    now = 0;
    passed = 0;
    durations = new double[frameLimit];
    start = future = past = std::chrono::high_resolution_clock::now();
}

FPSCounter::~FPSCounter() {
    delete[] durations;
}

double FPSCounter::getFPS() {
    return 1000000 * now / passed;
}

void FPSCounter::setFrameLimit(int newFrameLimit) {
    frameLimit = newFrameLimit;
    now = 0;
    passed = 0;
    delete[] durations;
    durations = new double[frameLimit];
    start = future = past = std::chrono::high_resolution_clock::now();
}

void FPSCounter::countFPSAndSleep() {
    future = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::duration duration = future - past;
    if (++now == frameLimit) {
        now = 0;
        passed = 0;
    }
    durations[now] = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    past = future;
    if (future - start >= std::chrono::seconds(1)) {
        start = future;
        now = 0;
        passed = 0;
        std::this_thread::sleep_until(start + std::chrono::nanoseconds(
                std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)).count() / frameLimit));
    } else if (now == 0) {
        start += std::chrono::seconds(1);
        passed = 0;
        std::this_thread::sleep_until(start);
    } else {
        passed += durations[now];
        std::this_thread::sleep_until(start + std::chrono::nanoseconds(
                std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)).count() / frameLimit *
                now));
    }
}

float FPSCounter::getDelta() {
    return durations[now];
}
