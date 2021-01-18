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
#include "util/Time.h"
#include "util/CLIConfig.h"
#include "window/LinuxWindow.hpp"
#include <engine/Application.h>

int main(int argc, char** argv) {
    int errorcode = CLIConfig::InitializeCLIConfig(argc, argv);
    if(errorcode != 0) return errorcode;
    // Start the engine
    auto engine = Application{};
    // Start the game
    Time::GameStart();
    engine.GameStart();
    return 0;
}
