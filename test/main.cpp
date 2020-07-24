#include <gtest/gtest.h>
// #include things
#include "eventstest.hpp"
#include "configtest.hpp"
#include "logging.hpp"
#include "newecstest.hpp"
#include "layertest.hpp"
#include "shaderprecompilertest.hpp"
#include "sanitytests.h"
#include "clitest.h"

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
