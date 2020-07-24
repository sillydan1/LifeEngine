#include <config/life_config>

#define BASECONFIG "./res/conf/testbase.conf"
using namespace life;

TEST(LifeConfigurationTest, ConfigurationGet) {
	// LifeConfig is singletonned
	auto a = LifeConfig::GetParser(BASECONFIG);
	int x, y = 0;
	a->get("windowdimentions", x, y);
	ASSERT_EQ(x, 900);
	ASSERT_EQ(y, 800);
}

TEST(LifeConfigurationTest, SimpleConfigurationGet) {
	int x, y;
	LIFE_CONF_GET(BASECONFIG)("windowdimentions", x ,y);
	ASSERT_EQ(x, 900);
	ASSERT_EQ(y, 800);
}

TEST(LifeConfigurationTest, ConfurationTypes) {
	int wrong_type = 0;
	LIFE_CONF_GET(BASECONFIG)("dxver", wrong_type);
	EXPECT_EQ(9, wrong_type); // TODO Should this be real?
	
	float right_type = 0.0f;
	LIFE_CONF_GET(BASECONFIG)("dxver", right_type);
	ASSERT_EQ(9.0f, right_type);
	
	double correct_type = 0.0;
	LIFE_CONF_GET(BASECONFIG)("dxver", correct_type);
	ASSERT_EQ(9.0, correct_type);
}

TEST(LifeConfigurationOutputTest, ConfigurationWrongOrder) {
	testing::internal::CaptureStdout();
	int k = 0;
	std::string s;
	LIFE_CONF_GET(BASECONFIG)("gfxAPI", k, s);
	// ASSERT VALUES
	ASSERT_EQ(0, k);
	ASSERT_EQ(std::string(), s);
	// OUTPUT ASSERTS
	std::string output = testing::internal::GetCapturedStdout();
	// something something with output has to be "[ERROR]:\t
}

TEST(LifeConfigurationOutputTest, ConfigurationSetTest) {
	int x = 1000, y = 1000;
	LIFE_CONF_SET(BASECONFIG)("windowdimentions", x, y);
	x = 200, y = 200;
	LIFE_CONF_GET(BASECONFIG)("windowdimentions", x ,y);
	ASSERT_EQ(1000, x);
	ASSERT_EQ(1000, y);
	x = 900, y = 800;
	LIFE_CONF_SET(BASECONFIG)("windowdimentions", x, y);
	LIFE_CONF_GET(BASECONFIG)("windowdimentions", x ,y);
	ASSERT_EQ(900, x);
	ASSERT_EQ(800, y);
}

TEST(LifeConfigurationOutputTest, StringTest) {
    char wtitle[30];
    LIFE_CONF_GET(BASECONFIG)("windowtitle", wtitle);
    ASSERT_STREQ("Life_Engine", wtitle);
}

#undef BASECONFIG
