#include <memory>
#include <kuma/default_shapes.hpp>

struct MoveSemanticsTest1 {
    static int dtor_calls;
    int data;
    MoveSemanticsTest1(int d = 0) : data(d) {}
    MoveSemanticsTest1(MoveSemanticsTest1&&) = default;
    MoveSemanticsTest1(const MoveSemanticsTest1&) = default;
    ~MoveSemanticsTest1() { dtor_calls++; }
};
int MoveSemanticsTest1::dtor_calls = 0;

void reset_move_dtor_calls() {
    MoveSemanticsTest1::dtor_calls = 0;
}

TEST(MoveSemanticsSanityTest1, SanityTests) {
    reset_move_dtor_calls();
    {
        MoveSemanticsTest1 a = MoveSemanticsTest1();
    }
    EXPECT_EQ(1, MoveSemanticsTest1::dtor_calls); // <-- This passes, makes sense
    reset_move_dtor_calls();
    {
        MoveSemanticsTest1 b = {3};
        auto a = std::make_shared<MoveSemanticsTest1>(std::move(b));
    }
    EXPECT_EQ(1, MoveSemanticsTest1::dtor_calls); // <-- This fails, why?
    reset_move_dtor_calls();
    {
        auto a = std::make_shared<MoveSemanticsTest1>(3);
    }
    EXPECT_EQ(1, MoveSemanticsTest1::dtor_calls); // <-- This fails, why?
    reset_move_dtor_calls();
    {
        MoveSemanticsTest1 b = {3};
        auto a = std::make_shared<MoveSemanticsTest1>(b);
    }
    EXPECT_EQ(2, MoveSemanticsTest1::dtor_calls); // <-- This passes, makes sense because of the copying
}

struct VtableTest1 {
    unsigned int k;
};

struct VtableTest2 {
    unsigned int k;
    virtual ~VtableTest2() = default;
};

TEST(VTableChangesTheSizeOfStructs, SanityTests) {
    EXPECT_EQ(sizeof(VtableTest1), sizeof(VtableTest2));
    VtableTest1 v1; 
    VtableTest2 v2;
    EXPECT_EQ(sizeof(v1), sizeof(v2));
}
#include <kuma/default_shapes.hpp>

TEST(IdentityMatricies, SanityTests) {
    glm::mat4 glm_ident{1.0f};
    glm::mat4 kumaident = life::kuma::default_shapes::mat4_identity_matrix;
    EXPECT_EQ(glm_ident, kumaident);
}

TEST(Time_pointMapping, SanityTests) {
    std::map<float, int> m = {{0.0f,1},{3.0f,2}};
    auto m1 = m.lower_bound(3.0f);
    auto m2 = m.lower_bound(6.0f);
    if(m2 == m.end()) m2 = m.begin(); // loop
    EXPECT_EQ(m1->second, m2->second + 1);
    auto x = m1->second;
    EXPECT_EQ(x, (m1++)->second);
}

TEST(Time_pointMapping2, SanityTests) {
    std::map<float, int> m = {{0.0f, 1}, {1.0f,2},{3.0f,3}};
    auto m1 = m.upper_bound(0.0f);
    EXPECT_EQ(2, m1->second);
    auto m2 = m.upper_bound(3.0f);
    ASSERT_EQ(m.end(), m2);
    // Somewhere in between
    {
        auto mb = m.upper_bound(0.5f);
        auto ma = mb; ma--;
        EXPECT_EQ(1, ma->second);
        EXPECT_EQ(2, mb->second);
    }
    {
        m2--;
        EXPECT_EQ(3, m2->second);
    }
    {
        auto mb = m.upper_bound(300.0f);
        mb--;
        EXPECT_EQ(3, mb->second);
    }
}

TEST(stdanySize, SanityTests) {
    ASSERT_EQ(sizeof(std::shared_ptr<int>), sizeof(std::shared_ptr<float>));
    ASSERT_EQ(sizeof(std::shared_ptr<MoveSemanticsTest1>), sizeof(std::shared_ptr<float>));
    ASSERT_EQ(sizeof(KumaTypes), sizeof(TYPE_PTR(float)));
}
