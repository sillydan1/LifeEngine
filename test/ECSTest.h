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
#ifndef LIFEENGINE_ECSTEST_H
#define LIFEENGINE_ECSTEST_H
#include <gtest/gtest.h>
#include <ecs/ecs>

template<typename T>
struct TestComponent : component<TestComponent<T>> {
    T value;
};

TEST(ECSTest, makeEntityPrimitives) {
    ecs world{};
    auto entity = world.make_entity<TestComponent<float>, TestComponent<int>, TestComponent<float>>();
    ASSERT_FLOAT_EQ(0.0f, entity.get_component<TestComponent<float>>()->value);
    ASSERT_FLOAT_EQ(0.0f, entity.get_component<TestComponent<float>>(1)->value);
    ASSERT_EQ(0, entity.get_component<TestComponent<int>>()->value);
}

#endif //LIFEENGINE_ECSTEST_H
