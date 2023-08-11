/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "model/userinput_history.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

namespace
{
    class UserInputHistoryTest : public ::testing::Test
    {
      protected:
        void SetUp() override
        {
            history.clear();
        }

        void TearDown() override
        {
            history.clear();
            std::filesystem::remove(temp_file);
        }

        std::filesystem::path temp_file{std::filesystem::temp_directory_path() / "test_history.txt"};
        model::UserInputHistory history{temp_file.string()};
    };

    TEST_F(UserInputHistoryTest, AppendValidInput)
    {
        history.append("test");
        ASSERT_EQ(history.previous(), "test");
    }

    TEST_F(UserInputHistoryTest, AppendEmptyInput)
    {
        history.append("");
        ASSERT_EQ(history.previous(), std::nullopt);
    }

    TEST_F(UserInputHistoryTest, AppendDuplicateInput)
    {
        history.append("test");
        history.append("test");
        ASSERT_EQ(history.previous(), "test");
        ASSERT_EQ(history.previous(), std::nullopt);
    }

    TEST_F(UserInputHistoryTest, AppendMultipleInputs)
    {
        history.append("test1");
        history.append("test2");
        history.append("test3");
        ASSERT_EQ(history.previous(), "test3");
        ASSERT_EQ(history.previous(), "test2");
        ASSERT_EQ(history.previous(), "test1");
        ASSERT_EQ(history.previous(), std::nullopt);
    }

    TEST_F(UserInputHistoryTest, NextOnEmptyHistory)
    {
        ASSERT_EQ(history.next(), std::nullopt);
    }

    TEST_F(UserInputHistoryTest, NextSingleElement)
    {
        history.append("test");
        ASSERT_EQ(history.previous(), "test");
        ASSERT_EQ(history.previous(), std::nullopt);
        ASSERT_EQ(history.next(), std::nullopt);

        history.append("test");
        ASSERT_EQ(history.previous(), "test");
        ASSERT_EQ(history.previous(), std::nullopt);
        ASSERT_EQ(history.next(), std::nullopt);
    }

    TEST_F(UserInputHistoryTest, NextMultipleElements)
    {
        history.append("test1");
        history.append("test2");
        history.append("test3");
        ASSERT_EQ(history.previous(), "test3");
        ASSERT_EQ(history.previous(), "test2");
        ASSERT_EQ(history.previous(), "test1");
        ASSERT_EQ(history.previous(), std::nullopt);
        ASSERT_EQ(history.next(), "test2");
        ASSERT_EQ(history.next(), "test3");
        ASSERT_EQ(history.next(), std::nullopt);
    }

    TEST_F(UserInputHistoryTest, NextAfterAppend)
    {
        history.append("test1");
        history.append("test2");
        ASSERT_EQ(history.previous(), "test2");
        ASSERT_EQ(history.previous(), "test1");
        history.append("test3");
        ASSERT_EQ(history.next(), std::nullopt);
    }

    TEST_F(UserInputHistoryTest, PreviousOnEmptyHistory)
    {
        ASSERT_EQ(history.previous(), std::nullopt);
    }

    TEST_F(UserInputHistoryTest, PreviousSingleElement)
    {
        history.append("test");
        ASSERT_EQ(history.previous(), "test");
        ASSERT_EQ(history.previous(), std::nullopt);

        history.append("test");
        ASSERT_EQ(history.previous(), "test");
        ASSERT_EQ(history.previous(), std::nullopt);
    }

    TEST_F(UserInputHistoryTest, PreviousMultipleElements)
    {
        history.append("test1");
        history.append("test2");
        history.append("test3");
        ASSERT_EQ(history.previous(), "test3");
        ASSERT_EQ(history.previous(), "test2");
        ASSERT_EQ(history.previous(), "test1");
        ASSERT_EQ(history.previous(), std::nullopt);
    }

    TEST_F(UserInputHistoryTest, PreviousAfterNext)
    {
        history.append("test1");
        history.append("test2");
        history.append("test3");
        ASSERT_EQ(history.previous(), "test3");
        ASSERT_EQ(history.previous(), "test2");
        ASSERT_EQ(history.next(), "test3");
        ASSERT_EQ(history.previous(), "test2");
    }

    TEST_F(UserInputHistoryTest, ClearEmptyHistory)
    {
        history.clear();
        ASSERT_EQ(history.previous(), std::nullopt);
    }

    TEST_F(UserInputHistoryTest, ClearNonEmptyHistory)
    {
        history.append("test1");
        history.append("test2");
        history.clear();
        ASSERT_EQ(history.previous(), std::nullopt);
        ASSERT_EQ(history.next(), std::nullopt);
    }

    TEST_F(UserInputHistoryTest, ClearAfterNavigation)
    {
        history.append("test1");
        history.append("test2");
        ASSERT_EQ(history.previous(), "test2");
        ASSERT_EQ(history.next(), std::nullopt);
        history.clear();
        ASSERT_EQ(history.previous(), std::nullopt);
        ASSERT_EQ(history.next(), std::nullopt);
    }

    TEST_F(UserInputHistoryTest, LoadNonEmptyHistory)
    {
        std::ofstream file{temp_file};
        file << "test1\n";
        file << "test2\n";
        file.close();

        history.load();
        ASSERT_EQ(history.previous(), "test2");
        ASSERT_EQ(history.previous(), "test1");
    }

    TEST_F(UserInputHistoryTest, SaveNonEmptyHistory)
    {
        history.append("test1");
        history.append("test2");
        history.save();

        ASSERT_TRUE(std::filesystem::exists(temp_file));
        std::ifstream file(temp_file);
        std::vector<std::string> lines;

        for (std::string line; std::getline(file, line);) {
            lines.push_back(line);
        }

        ASSERT_EQ(lines.size(), 2);
        ASSERT_EQ(lines[0], "test1");
        ASSERT_EQ(lines[1], "test2");
    }
}
