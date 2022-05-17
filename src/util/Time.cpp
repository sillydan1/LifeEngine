/**
 * Copyright (C) 2021 Asger Gitz-Johansen

   This file is part of lifeengine.

    lifeengine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    lifeengine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with lifeengine.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "Time.h"
#include <Timer.hpp>

Timer<float> frame_timer = Timer<float>();
Timer<double> timer = Timer<double>();
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
