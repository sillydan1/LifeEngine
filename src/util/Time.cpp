#include "Time.h"

Timer<float> frame_timer = Timer<float>();
Timer<double> timer = Timer<double>(); // TODO: Maybe a chrono::high_resolution_clock a bit too high-res for this?
double frame_time = 0.0;

double Time::GetGlobalTime() {
    return timer.seconds_elapsed();
}
float Time::GetFrameTime() {
    return frame_time;
}

void Time::GameStart() {
    timer.start();
}
void Time::FrameEnd() {
    frame_time = frame_timer.seconds_elapsed();
    frame_timer.start();
}
