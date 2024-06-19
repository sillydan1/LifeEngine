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
#include <clipp.h>
#include <spdlog/common.h>
#include "SANDBOX/Sandbox.h"
#include "config.h"

int main(const int argc, char* argv[]) {
    int verbosity = 0;
    bool version;
    auto cli = clipp::group(
        clipp::option("-v", "--vebosity").set(verbosity).doc("set verbosity [0-6]"),
        clipp::option("-V", "--version").set(version).doc("show version")
    );

    if(!clipp::parse(argc, argv, cli)) {
        std::cout << clipp::make_man_page(cli, argv[0]);
        return 1;
    }
    if(version) {
        std::cout << PROJECT_NAME << " " << PROJECT_VER << std::endl;
        return 0;
    }

    // Start the engine
    auto engine = Sandbox{};
    // Start the game
    engine.GameStart();
    return 0;
}
