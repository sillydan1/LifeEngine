#include <events/KeyEvents.hpp>

using namespace life;

TEST(KeyEventsTest, EventCategoryCheck) {
	KeyPressedEvent e(9, 0);
	ASSERT_TRUE(e.IsInCategory(EventCategory::EventCategoryInput));
	ASSERT_TRUE(e.IsInCategory(EventCategory::EventCategoryKeyboard));
	ASSERT_FALSE(e.IsInCategory(EventCategory::EventCategoryMouse));
	ASSERT_FALSE(e.IsInCategory(EventCategory::None));
	ASSERT_EQ(e.getKeyCode(), 9);
	KeyReleasedEvent e2(2);
	ASSERT_TRUE(e2.IsInCategory(EventCategory::EventCategoryInput));
	ASSERT_TRUE(e2.IsInCategory(EventCategory::EventCategoryKeyboard));
	ASSERT_FALSE(e2.IsInCategory(EventCategory::EventCategoryMouse));
	ASSERT_FALSE(e2.IsInCategory(EventCategory::None));
	ASSERT_EQ(e2.getKeyCode(), 2);
}

TEST(KeyEventsTest, EventTypesCheck) {
	KeyPressedEvent e(8, 0);
	ASSERT_EQ(e.GetEventType(), EventType::KeyPressed);
	ASSERT_NE(e.GetEventType(), EventType::KeyReleased);
}
// ============= EVENTS DISPATCHER TESTS ==================================== //
#define EVENTS_DISPATCHER_KEYVALUE_TEST 38291
#define EVENTS_DISPATCHER_REPEATVL_TEST 42
bool eventsDispatcherTest_FunctionCalling_Callback(KeyPressedEvent& context) {
	bool a = context.getKeyCode() 		== EVENTS_DISPATCHER_KEYVALUE_TEST;
	bool b = context.getRepeatCount() 	== EVENTS_DISPATCHER_REPEATVL_TEST;
	return a && b;
}
bool dispatcher_wrong(KeyReleasedEvent& context) {
	EXPECT_TRUE(false); // This function should never be called.
	return true;
}

TEST(EventsDispatcherTest, FunctionCalling) {
	KeyPressedEvent e(EVENTS_DISPATCHER_KEYVALUE_TEST, EVENTS_DISPATCHER_REPEATVL_TEST);
	KeyReleasedEvent e2(0);
	EventsDispatcher ed(e);
	ASSERT_TRUE(ed.dispatch<KeyPressedEvent>(eventsDispatcherTest_FunctionCalling_Callback));
	ASSERT_FALSE(ed.dispatch<KeyReleasedEvent>(dispatcher_wrong));
}
#undef EVENTS_DISPATCHER_KEYVALUE_TEST
#undef EVENTS_DISPATCHER_REPEATVL_TEST
