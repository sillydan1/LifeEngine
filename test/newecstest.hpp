#include <ecs/ecs>
// --- ENTITY CREATION --- //
struct FloatComponent : public life::ecs::Component<FloatComponent> {
	float data;
    static int dtor_calls;
	FloatComponent() = default;
	FloatComponent(float d) : data{d} {}
    ~FloatComponent() { dtor_calls++; }
};
struct IntComponent : public life::ecs::Component<IntComponent> {
	int data;
	static int dtor_calls;
	IntComponent() = default;
	IntComponent(int d) : data{d} {}
	~IntComponent() { dtor_calls++; }
};
int FloatComponent::dtor_calls = 0;
int IntComponent  ::dtor_calls = 0;
void reset_dtor_calls() {
    FloatComponent::dtor_calls = 0;
    IntComponent  ::dtor_calls = 0;
}

TEST(EntityCreationDefault, ECSTest) {
    reset_dtor_calls();
    {
        life::ecs::ECS m_ecs{};
        life::ecs::Entity e = m_ecs.makeEntity<FloatComponent, IntComponent>();
        ASSERT_EQ(0.f, e.GetComponent<FloatComponent>()->data);
        ASSERT_EQ(0, e.GetComponent<IntComponent>()->data);
    }
    ASSERT_EQ(1, FloatComponent::dtor_calls);
    ASSERT_EQ(1, IntComponent::dtor_calls);
}

TEST(EntityCreationParameterized, ECSTest) {
    reset_dtor_calls();
    {
        life::ecs::ECS m_ecs{};
        life::ecs::Entity e = m_ecs.makeEntity<FloatComponent, IntComponent>({32.f}, {128});

        ASSERT_EQ(32.f, e.GetComponent<FloatComponent>()->data);
        ASSERT_EQ(128, e.GetComponent<IntComponent>()->data);
    }
    ASSERT_EQ(1, FloatComponent::dtor_calls);
    ASSERT_EQ(1, IntComponent::dtor_calls);
}

TEST(EntityCreationParameterizedOneArgument, ECSTest) {
    reset_dtor_calls();
    {
        life::ecs::ECS m_ecs{};
        life::ecs::Entity e = m_ecs.makeEntity<FloatComponent, IntComponent>(32.f, 128);

        ASSERT_EQ(32.f, e.GetComponent<FloatComponent>()->data);
        ASSERT_EQ(128, e.GetComponent<IntComponent>()->data);
    }
    ASSERT_EQ(1, FloatComponent::dtor_calls);
    ASSERT_EQ(1, IntComponent::dtor_calls);
}

TEST(EntityCreationMultipleEntitySameComponentTypes, ECSTest) {
    reset_dtor_calls();
    {
        life::ecs::ECS m_ecs{};
        life::ecs::Entity e = m_ecs.makeEntity<FloatComponent, IntComponent>({32.f}, {128});
        life::ecs::Entity f = m_ecs.makeEntity<FloatComponent, FloatComponent>({16.f}, {10.f});

        ASSERT_EQ(32.f, e.GetComponent<FloatComponent>()->data);
        ASSERT_EQ(128, e.GetComponent<IntComponent>()->data);
        ASSERT_EQ(16.f, f.GetComponent<FloatComponent>(1)->data);
        ASSERT_EQ(10.f, f.GetComponent<FloatComponent>(0)->data);
    }
    ASSERT_EQ(3, FloatComponent::dtor_calls);
    ASSERT_EQ(1, IntComponent::dtor_calls);
}

TEST(EntityCreationMultipleComponentSameType, ECSTest) {
    reset_dtor_calls();
    {
        life::ecs::ECS m_ecs{};
        life::ecs::Entity f = m_ecs.makeEntity<FloatComponent, FloatComponent, FloatComponent, FloatComponent, FloatComponent>(
                {1}, {2}, {3}, {4}, {5});
        /// Please take note that components are put in like a stack. Not a heap
        EXPECT_EQ(1.f, f.GetComponent<FloatComponent>(4)->data);
        EXPECT_EQ(2.f, f.GetComponent<FloatComponent>(3)->data);
        EXPECT_EQ(3.f, f.GetComponent<FloatComponent>(2)->data);
        EXPECT_EQ(4.f, f.GetComponent<FloatComponent>(1)->data);
        EXPECT_EQ(5.f, f.GetComponent<FloatComponent>(0)->data);
    }
    ASSERT_EQ(5, FloatComponent::dtor_calls);
}

TEST(EntityCreationLValueReferences, ECSTest) {
    reset_dtor_calls();
    {
        life::ecs::ECS m_ecs{};
        FloatComponent fc = {32.f};
        IntComponent ic = {128};
        life::ecs::Entity e = m_ecs.makeEntity<FloatComponent, IntComponent>(std::move(fc), std::move(ic));
        life::ecs::Entity f = m_ecs.makeEntity<FloatComponent, IntComponent>({16.f}, {321});
        /// fc.data = 100.f; // The data is moved, so changing it will not make any difference.

        ASSERT_EQ(32.f, e.GetComponent<FloatComponent>()->data);
        ASSERT_EQ(128, e.GetComponent<IntComponent>()->data);
        ASSERT_EQ(16.f, f.GetComponent<FloatComponent>()->data);
        ASSERT_EQ(321, f.GetComponent<IntComponent>()->data);
    }
    ASSERT_EQ(2, FloatComponent::dtor_calls);
    ASSERT_EQ(2, IntComponent::dtor_calls);
}

TEST(ErroniousGetComponentCall, ECSTest) {
    reset_dtor_calls();
    {
        life::ecs::ECS m_ecs{};
        life::ecs::Entity e = m_ecs.makeEntity<FloatComponent, FloatComponent>({32.f}, {128.f});
        ASSERT_THROW(e.GetComponent<IntComponent>(), std::logic_error); // The entity has no IntComponents on it
        ASSERT_THROW(e.GetComponent<IntComponent>(0), std::logic_error); // The entity still has no InComponents
        ASSERT_THROW(e.GetComponent<FloatComponent>(3), std::out_of_range); // The entity has only 2 FloatComponents
    }
    ASSERT_EQ(2, FloatComponent::dtor_calls);
}

// --- SYSTEMS --- //
struct BasicTestSystem : public life::ecs::System {

    BasicTestSystem() {
        AddComponentType(FloatComponent::ID); // We only operate on float components
        FinalizeComponentAdding();
    }
    void Update(const std::shared_ptr<life::ecs::BaseComponent>& components) override {
        dynamic_cast<FloatComponent&>(*components).data += 10.f;
    }
};

struct AdvancedTestSystem : public life::ecs::System {
    AdvancedTestSystem() {
        AddComponentType(FloatComponent::ID);
        AddComponentType(IntComponent::ID);
        FinalizeComponentAdding();
    }
    void UpdateMultiple(const std::vector<std::shared_ptr<life::ecs::BaseComponent>>& components) override {
        ASSERT_EQ(2, components.size());
        /// Note: The list will always be sorted according to the Components' ID. This might bite me in the ass later though
        for(auto& c : components) {
            if(c->GetID() == FloatComponent::ID)
                dynamic_cast<FloatComponent&>(*c).data += 20.f;
            else
                dynamic_cast<IntComponent&>(*c).data += 10;
        }
    }
};

TEST(BasicSystemInstantiation, ECSTest) {
    reset_dtor_calls();
    {
        life::ecs::ECS m_ecs{};
        life::ecs::Entity e = m_ecs.makeEntity<FloatComponent>({32.f});
        life::ecs::Entity f = m_ecs.makeEntity<FloatComponent>();
        life::ecs::Entity g = m_ecs.makeEntity<FloatComponent, FloatComponent>({1.0f}, {83.0f});
        life::ecs::Entity h = m_ecs.makeEntity<FloatComponent, IntComponent>({0.0f}, {2});

        life::ecs::SystemList list_one{};
        list_one.PushSystem(BasicTestSystem());

        m_ecs.UpdateSystems(list_one);
        EXPECT_EQ(42.f, e.GetComponent<FloatComponent>()->data);

        EXPECT_EQ(10.f, f.GetComponent<FloatComponent>()->data);

        EXPECT_EQ(93.0f, g.GetComponent<FloatComponent>(0)->data);
        EXPECT_EQ(11.0f, g.GetComponent<FloatComponent>(1)->data);

        EXPECT_EQ(2, h.GetComponent<IntComponent>()->data);
        EXPECT_EQ(10.0f, h.GetComponent<FloatComponent>()->data);
    }
    ASSERT_EQ(5, FloatComponent::dtor_calls);
    ASSERT_EQ(1, IntComponent::dtor_calls);
}

TEST(AdvancedSystemInstantiation, ECSTest) {
    reset_dtor_calls();
    {
        life::ecs::ECS m_ecs{};
        life::ecs::Entity e = m_ecs.makeEntity<FloatComponent>({32.f});
        life::ecs::Entity f = m_ecs.makeEntity<FloatComponent>();
        life::ecs::Entity g = m_ecs.makeEntity<FloatComponent, FloatComponent>({1.0f}, {83.0f});
        life::ecs::Entity h = m_ecs.makeEntity<IntComponent, FloatComponent>({2}, {0.0f});
        life::ecs::Entity i = m_ecs.makeEntity<FloatComponent, IntComponent>({0.0f}, {2});

        life::ecs::SystemList list_one{};
        list_one.PushSystem(AdvancedTestSystem());

        m_ecs.UpdateSystems(list_one);
        EXPECT_EQ(32.f, e.GetComponent<FloatComponent>()->data);

        EXPECT_EQ(0.f, f.GetComponent<FloatComponent>()->data);

        EXPECT_EQ(83.0f, g.GetComponent<FloatComponent>(0)->data);
        EXPECT_EQ(1.0f, g.GetComponent<FloatComponent>(1)->data);

        EXPECT_EQ(12, h.GetComponent<IntComponent>()->data);
        EXPECT_EQ(20.0f, h.GetComponent<FloatComponent>()->data);

        EXPECT_EQ(12, i.GetComponent<IntComponent>()->data);
        EXPECT_EQ(20.0f, i.GetComponent<FloatComponent>()->data);
    }
    ASSERT_EQ(6, FloatComponent::dtor_calls);
    ASSERT_EQ(2, IntComponent::dtor_calls);
}

struct AdvancedFlippedTestSystem : public life::ecs::System {
    AdvancedFlippedTestSystem() {
        AddComponentType(IntComponent::ID);
        AddComponentType(FloatComponent::ID);
        FinalizeComponentAdding();
    }
};

TEST(HashCombination, ECSTest) {
    AdvancedFlippedTestSystem a{};
    AdvancedTestSystem b{};
    ASSERT_EQ(a.component_filter, b.component_filter);
}




