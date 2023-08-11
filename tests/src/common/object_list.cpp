/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "common/object_list.hpp"
#include <gtest/gtest.h>

namespace
{
    class ObjectListTest : public ::testing::Test
    {
      protected:
        void SetUp() override
        {
            list.init();
        }

        common::ObjectList list;
    };

    TEST_F(ObjectListTest, Init)
    {
        EXPECT_EQ(list.size(), 0);
    }

    TEST_F(ObjectListTest, Add)
    {
        auto x = 5;
        list.add(&x);

        EXPECT_EQ(list.size(), 1);
        EXPECT_EQ(list.first(), &x);
    }

    TEST_F(ObjectListTest, RemoveEmptyList)
    {
        auto x = 5;
        EXPECT_FALSE(list.remove(&x));
    }

    TEST_F(ObjectListTest, RemoveNonExistentObject)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);

        EXPECT_FALSE(list.remove(&y));
    }

    TEST_F(ObjectListTest, RemoveExistingObject)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);
        list.add(&y);

        EXPECT_TRUE(list.remove(&x));
        EXPECT_EQ(list.size(), 1);
        EXPECT_EQ(list.first(), &y);
    }

    TEST_F(ObjectListTest, RemoveCurrent)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);
        list.add(&y);
        list.first();

        EXPECT_TRUE(list.remove(&x));
        EXPECT_EQ(list.next(), &y);
    }

    TEST_F(ObjectListTest, Clear)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);
        list.add(&y);
        list.clear(false);

        EXPECT_EQ(list.size(), 0);
    }

    TEST_F(ObjectListTest, FirstEmptyList)
    {
        EXPECT_EQ(list.first(), nullptr);
    }

    TEST_F(ObjectListTest, FirstNonEmptyList)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);
        list.add(&y);

        EXPECT_EQ(list.first(), &x);
    }

    TEST_F(ObjectListTest, NextEmptyList)
    {
        EXPECT_EQ(list.next(), nullptr);
    }

    TEST_F(ObjectListTest, NextNonEmptyList)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);
        list.add(&y);
        list.first();

        EXPECT_EQ(list.next(), &y);
        EXPECT_EQ(list.next(), nullptr);
    }

    TEST_F(ObjectListTest, SizeEmptyList)
    {
        EXPECT_EQ(list.size(), 0);
    }

    TEST_F(ObjectListTest, SizeNonEmptyList)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);
        list.add(&y);

        EXPECT_EQ(list.size(), 2);
    }

    TEST_F(ObjectListTest, ContainsEmptyList)
    {
        auto x = 5;
        EXPECT_FALSE(list.contains(&x));
    }

    TEST_F(ObjectListTest, ContainsNonExistentObject)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);

        EXPECT_FALSE(list.contains(&y));
    }

    TEST_F(ObjectListTest, ContainsExistingObject)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);
        list.add(&y);

        EXPECT_TRUE(list.contains(&x));
    }

    TEST_F(ObjectListTest, EmptyEmptyList)
    {
        EXPECT_TRUE(list.empty());
    }

    TEST_F(ObjectListTest, EmptyNonEmptyList)
    {
        auto x = 5;
        list.add(&x);

        EXPECT_FALSE(list.empty());
    }

    TEST_F(ObjectListTest, AddHead)
    {
        auto x = 5;
        auto y = 6;

        list.add_head(&x);
        list.add_head(&y);

        EXPECT_EQ(list.size(), 2);
        EXPECT_EQ(list.first(), &y);
    }

    TEST_F(ObjectListTest, AddHeadCurrent)
    {
        auto x = 5;
        auto y = 6;
        auto z = 7;

        list.add(&x);
        list.add(&y);
        list.first();
        list.add_head(&z);

        EXPECT_EQ(list.next(), &y);
    }

    TEST_F(ObjectListTest, AddTail)
    {
        auto x = 5;
        auto y = 6;

        list.add_tail(&x);
        list.add_tail(&y);

        EXPECT_EQ(list.size(), 2);
        EXPECT_EQ(list.first(), &x);
    }

    TEST_F(ObjectListTest, RemoveHeadEmptyList)
    {
        EXPECT_EQ(list.remove_head(), nullptr);
    }

    TEST_F(ObjectListTest, RemoveHeadNonEmptyList)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);
        list.add(&y);

        EXPECT_EQ(list.remove_head(), &x);
        EXPECT_EQ(list.size(), 1);
        EXPECT_EQ(list.first(), &y);
    }

    TEST_F(ObjectListTest, RemoveHeadCurrent)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);
        list.add(&y);
        list.first();

        EXPECT_EQ(list.remove_head(), &x);
        EXPECT_EQ(list.next(), &y);
    }

    TEST_F(ObjectListTest, RemoveTailEmptyList)
    {
        EXPECT_EQ(list.remove_tail(), nullptr);
    }

    TEST_F(ObjectListTest, RemoveTailNonEmptyList)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);
        list.add(&y);

        EXPECT_EQ(list.remove_tail(), &y);
        EXPECT_EQ(list.size(), 1);
        EXPECT_EQ(list.first(), &x);
    }

    TEST_F(ObjectListTest, RemoveTailCurrent)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);
        list.add(&y);
        list.first();
        list.next();

        EXPECT_EQ(list.remove_tail(), &y);
        EXPECT_EQ(list.next(), nullptr);
    }

    TEST_F(ObjectListTest, OperatorSquareBracketsNonConst)
    {
        auto x = 5;
        auto y = 6;
        auto z = 7;

        list.add(&x);
        list.add(&y);

        EXPECT_EQ(list[0], &x);
        EXPECT_EQ(list[1], &y);

        list[1] = &z;
        EXPECT_EQ(list[1], &z);
    }

    TEST_F(ObjectListTest, OperatorSquareBracketsConst)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);
        list.add(&y);
        const auto& const_list = list;

        EXPECT_EQ(const_list[0], &x);
        EXPECT_EQ(const_list[1], &y);
    }

    TEST_F(ObjectListTest, Iterators)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);
        list.add(&y);

        auto it = list.begin();
        EXPECT_EQ(*it, &x);

        ++it;
        EXPECT_EQ(*it, &y);

        ++it;
        EXPECT_EQ(it, list.end());

        auto rit = list.rbegin();
        EXPECT_EQ(*rit, &y);

        ++rit;
        EXPECT_EQ(*rit, &x);

        ++rit;
        EXPECT_EQ(rit, list.rend());
    }

    TEST_F(ObjectListTest, EraseSingleElement)
    {
        auto x = 5;
        auto y = 6;

        list.add(&x);
        list.add(&y);
        list.erase(list.begin());

        EXPECT_EQ(list.size(), 1);
        EXPECT_EQ(list.first(), &y);
    }

    TEST_F(ObjectListTest, EraseRange)
    {
        auto x = 5;
        auto y = 6;
        auto z = 7;

        list.add(&x);
        list.add(&y);
        list.add(&z);
        list.erase(list.begin(), std::next(list.begin(), 2));

        EXPECT_EQ(list.size(), 1);
        EXPECT_EQ(list.first(), &z);
    }

    TEST_F(ObjectListTest, Sort)
    {
        auto x = 5;
        auto y = 6;
        auto z = 7;

        list.add(&x);
        list.add(&y);
        list.add(&z);

        list.sort([](auto* const a, auto* const b) {
            return *static_cast<int*>(a) > *static_cast<int*>(b);
        });

        auto it = list.begin();
        EXPECT_EQ(*it, &z);

        ++it;
        EXPECT_EQ(*it, &y);

        ++it;
        EXPECT_EQ(*it, &x);
    }
}
