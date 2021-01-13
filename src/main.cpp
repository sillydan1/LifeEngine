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
#include <util/CLIConfig.h>

int main(int argc, char** argv) {
    // Initialize CLI configuration (based on CLI Args)
    CLIConfig::getInstance().ParseCLIOptionsAndCheckForRequirements(argc, argv);
    if(CLIConfig::getInstance().GetStatusCode() != EXIT_SUCCESS || CLIConfig::getInstance()["help"]) {
        CLIConfig::getInstance().PrintHelpMessage(argv);
        return CLIConfig::getInstance().GetStatusCode();
    }
    if(CLIConfig::getInstance()["verbosity"])
        spdlog::set_level(static_cast<spdlog::level::level_enum>(6-CLIConfig::getInstance()["verbosity"].as_integer()));
    else
        spdlog::set_level(spdlog::level::level_enum::warn);
    ///////////////////////////////////////////////////////////////////
    // Start the engine

    // Start the game

    return 0;
}
