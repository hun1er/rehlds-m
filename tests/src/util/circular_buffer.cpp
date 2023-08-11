/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "util/circular_buffer.hpp"
#include <gtest/gtest.h>

namespace
{
    class CircularBufferTest : public ::testing::Test
    {
      protected:
        util::CircularBuffer<int, 3> buffer{};
    };

    class CircularBufferTestString : public ::testing::Test
    {
      protected:
        util::CircularBuffer<std::string, 3> buffer{};
    };

    TEST_F(CircularBufferTest, Front)
    {
        // Test front on an empty buffer
        ASSERT_EQ(buffer.front(), 0);

        buffer.push_back(1);
        ASSERT_EQ(buffer.front(), 1);

        buffer.push_back(2);
        ASSERT_EQ(buffer.front(), 1);

        buffer.push_back(3);
        ASSERT_EQ(buffer.front(), 1);

        // Test front after popping an element
        buffer.pop_front();
        ASSERT_EQ(buffer.front(), 2);
    }

    TEST_F(CircularBufferTest, Back)
    {
        // Test back on an empty buffer
        ASSERT_EQ(buffer.back(), 0);

        buffer.push_back(1);
        ASSERT_EQ(buffer.back(), 1);

        buffer.push_back(2);
        ASSERT_EQ(buffer.back(), 2);

        buffer.push_back(3);
        ASSERT_EQ(buffer.back(), 3);

        // Test back after popping an element
        buffer.pop_back();
        ASSERT_EQ(buffer.back(), 2);
    }

    TEST_F(CircularBufferTest, Size)
    {
        // Test size on an empty buffer
        ASSERT_EQ(buffer.size(), 0);

        // Test size on a non-empty buffer
        buffer.push_back(1);
        ASSERT_EQ(buffer.size(), 1);

        buffer.push_back(2);
        ASSERT_EQ(buffer.size(), 2);

        buffer.push_back(3);
        ASSERT_EQ(buffer.size(), 3);

        // Test size after popping an element
        buffer.pop_front();
        ASSERT_EQ(buffer.size(), 2);
    }

    TEST_F(CircularBufferTest, PushFront)
    {
        // Test pushing to an empty buffer
        buffer.push_front(1);
        ASSERT_EQ(buffer.front(), 1);
        ASSERT_EQ(buffer.back(), 1);
        ASSERT_EQ(buffer.size(), 1);

        // Test pushing to a non-full buffer
        buffer.push_front(2);
        ASSERT_EQ(buffer.front(), 2);
        ASSERT_EQ(buffer.back(), 1);
        ASSERT_EQ(buffer.size(), 2);

        // Test pushing to a full buffer
        buffer.push_front(3);
        ASSERT_EQ(buffer.front(), 3);
        ASSERT_EQ(buffer.back(), 1);
        ASSERT_EQ(buffer.size(), 3);

        // Test pushing to a full buffer, causing overwrite
        buffer.push_front(4);
        ASSERT_EQ(buffer.front(), 4);
        ASSERT_EQ(buffer.back(), 2);
        ASSERT_EQ(buffer.size(), 3);
    }

    TEST_F(CircularBufferTestString, EmplaceFront)
    {
        // Test emplacing to an empty buffer
        buffer.emplace_front(5, 'a');
        ASSERT_EQ(buffer.front(), "aaaaa");
        ASSERT_EQ(buffer.back(), "aaaaa");
        ASSERT_EQ(buffer.size(), 1);

        // Test emplacing to a non-full buffer
        buffer.emplace_front(3, 'b');
        ASSERT_EQ(buffer.front(), "bbb");
        ASSERT_EQ(buffer.back(), "aaaaa");
        ASSERT_EQ(buffer.size(), 2);

        // Test emplacing to a full buffer
        buffer.emplace_front(2, 'c');
        ASSERT_EQ(buffer.front(), "cc");
        ASSERT_EQ(buffer.back(), "aaaaa");
        ASSERT_EQ(buffer.size(), 3);

        // Test emplacing to a full buffer, causing overwrite
        buffer.emplace_front(4, 'd');
        ASSERT_EQ(buffer.front(), "dddd");
        ASSERT_EQ(buffer.back(), "bbb");
        ASSERT_EQ(buffer.size(), 3);
    }

    TEST_F(CircularBufferTest, PushBack)
    {
        // Test pushing to an empty buffer
        buffer.push_back(1);
        ASSERT_EQ(buffer.front(), 1);
        ASSERT_EQ(buffer.back(), 1);
        ASSERT_EQ(buffer.size(), 1);

        // Test pushing to a non-full buffer
        buffer.push_back(2);
        ASSERT_EQ(buffer.front(), 1);
        ASSERT_EQ(buffer.back(), 2);
        ASSERT_EQ(buffer.size(), 2);

        // Test pushing to a full buffer
        buffer.push_back(3);
        ASSERT_EQ(buffer.front(), 1);
        ASSERT_EQ(buffer.back(), 3);
        ASSERT_EQ(buffer.size(), 3);

        // Test pushing to a full buffer, causing overwrite
        buffer.push_back(4);
        ASSERT_EQ(buffer.front(), 2);
        ASSERT_EQ(buffer.back(), 4);
        ASSERT_EQ(buffer.size(), 3);
    }

    TEST_F(CircularBufferTestString, EmplaceBack)
    {
        // Test emplacing to an empty buffer
        buffer.emplace_back(5, 'a');
        ASSERT_EQ(buffer.front(), "aaaaa");
        ASSERT_EQ(buffer.back(), "aaaaa");
        ASSERT_EQ(buffer.size(), 1);

        // Test emplacing to a non-full buffer
        buffer.emplace_back(3, 'b');
        ASSERT_EQ(buffer.front(), "aaaaa");
        ASSERT_EQ(buffer.back(), "bbb");
        ASSERT_EQ(buffer.size(), 2);

        // Test emplacing to a full buffer
        buffer.emplace_back(2, 'c');
        ASSERT_EQ(buffer.front(), "aaaaa");
        ASSERT_EQ(buffer.back(), "cc");
        ASSERT_EQ(buffer.size(), 3);

        // Test emplacing to a full buffer, causing overwrite
        buffer.emplace_back(4, 'd');
        ASSERT_EQ(buffer.front(), "bbb");
        ASSERT_EQ(buffer.back(), "dddd");
        ASSERT_EQ(buffer.size(), 3);
    }

    TEST_F(CircularBufferTest, PopFront)
    {
        // Test popping from an empty buffer
        buffer.pop_front();
        ASSERT_EQ(buffer.size(), 0);

        // Test popping from a non-empty buffer
        buffer.push_back(1);
        buffer.push_back(2);
        buffer.push_back(3);
        ASSERT_EQ(buffer.size(), 3);

        buffer.pop_front();
        ASSERT_EQ(buffer.front(), 2);
        ASSERT_EQ(buffer.size(), 2);

        buffer.pop_front();
        ASSERT_EQ(buffer.front(), 3);
        ASSERT_EQ(buffer.size(), 1);

        buffer.pop_front();
        ASSERT_EQ(buffer.size(), 0);
    }

    TEST_F(CircularBufferTest, PopBack)
    {
        // Test popping from an empty buffer
        buffer.pop_back();
        ASSERT_EQ(buffer.size(), 0);

        // Test popping from a non-empty buffer
        buffer.push_back(1);
        buffer.push_back(2);
        buffer.push_back(3);
        ASSERT_EQ(buffer.size(), 3);

        buffer.pop_back();
        ASSERT_EQ(buffer.back(), 2);
        ASSERT_EQ(buffer.size(), 2);

        buffer.pop_back();
        ASSERT_EQ(buffer.back(), 1);
        ASSERT_EQ(buffer.size(), 1);

        buffer.pop_back();
        ASSERT_EQ(buffer.size(), 0);
    }

    TEST_F(CircularBufferTest, Full)
    {
        // Test full on an empty buffer
        ASSERT_FALSE(buffer.full());

        // Test full on a non-full buffer
        buffer.push_back(1);
        buffer.push_back(2);
        ASSERT_FALSE(buffer.full());

        // Test full on a full buffer
        buffer.push_back(3);
        ASSERT_TRUE(buffer.full());

        // Test full after popping an element
        buffer.pop_front();
        ASSERT_FALSE(buffer.full());
    }

    TEST_F(CircularBufferTest, Empty)
    {
        // Test empty on an empty buffer
        ASSERT_TRUE(buffer.empty());

        // Test empty on a non-empty buffer
        buffer.push_back(1);
        ASSERT_FALSE(buffer.empty());

        // Test empty after clearing the buffer
        buffer.clear();
        ASSERT_TRUE(buffer.empty());
    }

    TEST_F(CircularBufferTest, MaxSize)
    {
        // Test max_size on an empty buffer
        ASSERT_EQ(buffer.max_size(), 3);

        // Test max_size on a non-empty buffer
        buffer.push_back(1);
        ASSERT_EQ(buffer.max_size(), 3);

        // Test max_size on a full buffer
        buffer.push_back(2);
        buffer.push_back(3);
        ASSERT_EQ(buffer.max_size(), 3);

        // Test max_size after popping an element
        buffer.pop_front();
        ASSERT_EQ(buffer.max_size(), 3);
    }

    TEST_F(CircularBufferTest, Clear)
    {
        // Test clearing an empty buffer
        buffer.clear();
        ASSERT_EQ(buffer.size(), 0);

        // Test clearing a non-empty buffer
        buffer.push_back(1);
        buffer.push_back(2);
        buffer.push_back(3);
        ASSERT_EQ(buffer.size(), 3);

        buffer.clear();
        ASSERT_EQ(buffer.size(), 0);
    }

    TEST_F(CircularBufferTest, BracketOperator)
    {
        // Test operator[] on an empty buffer
        ASSERT_EQ(buffer[0], 0);
        ASSERT_EQ(buffer[1], 0);
        ASSERT_EQ(buffer[2], 0);

        buffer.push_back(1);
        ASSERT_EQ(buffer[0], 1);

        buffer.push_back(2);
        ASSERT_EQ(buffer[0], 1);
        ASSERT_EQ(buffer[1], 2);

        buffer.push_back(3);
        ASSERT_EQ(buffer[0], 1);
        ASSERT_EQ(buffer[1], 2);
        ASSERT_EQ(buffer[2], 3);

        buffer.push_back(4);
        ASSERT_EQ(buffer[0], 2);
        ASSERT_EQ(buffer[1], 3);
        ASSERT_EQ(buffer[2], 4);

        // Test operator[] after popping an element from the front
        buffer.pop_front();
        ASSERT_EQ(buffer[0], 3);
        ASSERT_EQ(buffer[1], 4);

        // Test operator[] after popping an element from the back
        buffer.pop_back();
        ASSERT_EQ(buffer[0], 3);
    }

    TEST_F(CircularBufferTest, ConstBracketOperator)
    {
        const auto& const_buffer = buffer;

        // Test operator[] on an empty buffer
        ASSERT_EQ(const_buffer[0], 0);
        ASSERT_EQ(const_buffer[1], 0);
        ASSERT_EQ(const_buffer[2], 0);

        buffer.push_back(1);
        ASSERT_EQ(const_buffer[0], 1);

        buffer.push_back(2);
        ASSERT_EQ(const_buffer[0], 1);
        ASSERT_EQ(const_buffer[1], 2);

        buffer.push_back(3);
        ASSERT_EQ(const_buffer[0], 1);
        ASSERT_EQ(const_buffer[1], 2);
        ASSERT_EQ(const_buffer[2], 3);

        // Test operator[] after popping an element from the front
        buffer.pop_front();
        ASSERT_EQ(const_buffer[0], 2);
        ASSERT_EQ(const_buffer[1], 3);

        // Test operator[] after popping an element from the back
        buffer.pop_back();
        ASSERT_EQ(const_buffer[0], 2);
    }
}
