#include <layers/Layer.hpp>

using namespace life;

class UnitTestLayer : public Layer {
public:
	UnitTestLayer() : Layer("TestLayer") {}
	void OnEvent(Event &event) override {
        event.SetHandled();
	}
};

TEST(LayerUsage, SimpleUsageTest) {
	UnitTestLayer mytestlayer;
	KeyPressedEvent e(9, 0);
	mytestlayer.OnEvent(e);
	ASSERT_TRUE(e.IsHandled());
}

#include <layers/LayerStack.hpp>

TEST(LayerStackUsage, SimpleUsageTest) {
	LayerStack mytestlayerstack;
	mytestlayerstack.PushLayer(new UnitTestLayer());
	ASSERT_EQ(1, mytestlayerstack.size());
	KeyPressedEvent e(2, 0);
	bool was_handled = false;
	for (auto l : mytestlayerstack) {
		l->OnEvent(e);
		if (e.IsHandled()) {
			was_handled = true;
			break;
		}
	}
	ASSERT_TRUE(was_handled);
}
