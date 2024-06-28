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
#ifndef LIFEENGINE_DITEST_H
#define LIFEENGINE_DITEST_H
#include <gtest/gtest.h>
#include <di/DI.h>

TEST(DITest, BasicUsage) {
    di::set<float>(30.0f);
    ASSERT_FLOAT_EQ(30.0f, di::get<float>());
}

TEST(DITest, BasicUsageRefferences) {
    auto ff = float(30.0f);
    di::set<float&>(ff);
    ASSERT_FLOAT_EQ(30.0f, di::get<float&>());
}

TEST(DITest, BasicUsagePointers) {
    auto* ff = new float(30.0f);
    di::set<float*>(ff);
    EXPECT_EQ(ff, di::get<float*>());
    ASSERT_FLOAT_EQ(30.0f, *di::get<float*>());
    delete ff;
}

#endif //LIFEENGINE_DITEST_H
