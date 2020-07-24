#include <life_core.h>

TEST(LoggingTest, ColorLogging) {
	testing::internal::CaptureStdout();
	LIFE_LOG("Hest");
	std::string output = testing::internal::GetCapturedStdout();
#ifndef WIN32
	ASSERT_EQ(std::string("\x1B[1;37m[LOG]:\t\x1B[0mHest\n"), output);
#endif
}

#include <string>

TEST(LoggingTest, FormatLogging) {
	testing::internal::CaptureStdout();
	std::string hello_what = "world";
	LIFE_LOG("Hello %s", hello_what.c_str());
	std::string output = testing::internal::GetCapturedStdout();
#ifndef WIN32
	ASSERT_EQ(std::string("\x1B[1;37m[LOG]:\t\x1B[0mHello world\n"), output);
#endif
}
