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
#include <lyra/lyra.hpp>
#include <spdlog/common.h>
#include "SANDBOX/Sandbox.h"
#include "config.h"

int main(const int argc, char* argv[]) {
    int verbosity = SPDLOG_LEVEL_WARN;
    bool show_version = false;
    bool show_help = false;
    auto cli = lyra::cli()
        | lyra::help(show_help).description("A work in progress game engine")
            ("show this message")
        | lyra::opt(verbosity, "0-6")
            ["-v"]["--verbosity"]("set verbosity level, default: " + std::to_string(verbosity))
        | lyra::opt(show_version)
            ["-V"]["--version"]("show version")
        ;
    auto args = cli.parse({argc, argv});
    if(!args) {
        spdlog::error(args.message());
        return 1;
    }
    if(show_help) {
        std::cout << cli << std::endl;
        return 0;
    }
    if(show_version) {
        std::cout << PROJECT_NAME << " " << PROJECT_VER << std::endl;
        return 0;
    }
    spdlog::set_level(static_cast<spdlog::level::level_enum>(std::max(SPDLOG_LEVEL_OFF - verbosity, 0)));
    spdlog::debug("log level set to {}", verbosity);
    auto engine = sandbox{};
    engine.GameStart();
    return 0;
}
