#ifndef LIFE_ENGINE_CLITEST_H
#define LIFE_ENGINE_CLITEST_H
#include <gtest/gtest.h>
#include <command-line-interface/cli.h>

LIFE_CLI_FUNCTION(TestFunction, "Internal testing function. Essentially does nothing") {
	life::cli::CLI{} << "TestFunction Fired!";
	return life::cli::CliReturnValue::Success;
}

TEST(LifeCliTest, BasicSystemTest) {
	std::shared_ptr<std::stringstream> test_stream{};
	life::cli::CliReturnValue returnValue;
	life::cli::ExecuteCLICommand("TestFunction", test_stream, &returnValue);
	ASSERT_EQ(returnValue, life::cli::CliReturnValue::Success);
	ASSERT_STREQ(test_stream->str().c_str(), "TestFunction Fired!");
}

#endif //LIFE_ENGINE_CLITEST_H
