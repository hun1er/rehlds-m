/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "model/cmdline_args.hpp"
#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

namespace
{
    class CmdLineArgsTest : public ::testing::Test
    {
      protected:
        static constexpr std::size_t ARGC = 10;

        static constexpr std::array<const char*, ARGC> ARGV = {
          "hlds.exe", "-game", "cstrike", "-tos", "-pingboost", "2", "+maxplayers", "32", "+map", "de_dust2"};

        model::CmdLineArgs cmdline_args{static_cast<int>(ARGC), ARGV.data()};
    };

    TEST_F(CmdLineArgsTest, GetCurrent)
    {
        EXPECT_STREQ(cmdline_args.get_current(), "-game cstrike -tos -pingboost 2 +maxplayers 32 +map de_dust2");
    }

    TEST_F(CmdLineArgsTest, GetCurrentEmpty)
    {
        constexpr std::array<const char*, 0> argv{};
        const model::CmdLineArgs args(static_cast<int>(argv.size()), argv.data());
        EXPECT_STREQ(args.get_current(), "");
    }

    TEST_F(CmdLineArgsTest, PushFront)
    {
        cmdline_args.push_front("-newparam", "newoption");
        EXPECT_STREQ(cmdline_args.get_current(),
                     "-newparam newoption -game cstrike -tos -pingboost 2 +maxplayers 32 +map de_dust2");
    }

    TEST_F(CmdLineArgsTest, PushFrontNoOption)
    {
        cmdline_args.push_front("-newparam");
        EXPECT_STREQ(cmdline_args.get_current(),
                     "-newparam -game cstrike -tos -pingboost 2 +maxplayers 32 +map de_dust2");
    }

    TEST_F(CmdLineArgsTest, PushFrontExistingParam)
    {
        cmdline_args.push_front("+pingboost", "5");
        EXPECT_STREQ(cmdline_args.get_current(), "+pingboost 5 -game cstrike -tos +maxplayers 32 +map de_dust2");
    }

    TEST_F(CmdLineArgsTest, PushBack)
    {
        cmdline_args.push_back("-newparam", "newoption");
        EXPECT_STREQ(cmdline_args.get_current(),
                     "-game cstrike -tos -pingboost 2 +maxplayers 32 +map de_dust2 -newparam newoption");
    }

    TEST_F(CmdLineArgsTest, PushBackNoOption)
    {
        cmdline_args.push_back("-newparam");
        EXPECT_STREQ(cmdline_args.get_current(),
                     "-game cstrike -tos -pingboost 2 +maxplayers 32 +map de_dust2 -newparam");
    }

    TEST_F(CmdLineArgsTest, PushBackExistingParam)
    {
        cmdline_args.push_back("+pingboost", "5");
        EXPECT_STREQ(cmdline_args.get_current(), "-game cstrike -tos +maxplayers 32 +map de_dust2 +pingboost 5");
    }

    TEST_F(CmdLineArgsTest, Remove)
    {
        cmdline_args.remove("game");
        EXPECT_STREQ(cmdline_args.get_current(), "-tos -pingboost 2 +maxplayers 32 +map de_dust2");
    }

    TEST_F(CmdLineArgsTest, RemoveMultipleParams)
    {
        cmdline_args.remove("+tos");
        cmdline_args.remove("-maxplayers");
        EXPECT_STREQ(cmdline_args.get_current(), "-game cstrike -pingboost 2 +map de_dust2");
    }

    TEST_F(CmdLineArgsTest, RemoveNonExistentParam)
    {
        cmdline_args.remove("-nonexistent");
        EXPECT_STREQ(cmdline_args.get_current(), "-game cstrike -tos -pingboost 2 +maxplayers 32 +map de_dust2");
    }

    TEST_F(CmdLineArgsTest, Contains)
    {
        EXPECT_TRUE(cmdline_args.contains("-game"));
    }

    TEST_F(CmdLineArgsTest, ContainsAfterRemove)
    {
        cmdline_args.remove("-game");
        EXPECT_FALSE(cmdline_args.contains("-game"));
    }

    TEST_F(CmdLineArgsTest, ContainsNonExistentParam)
    {
        EXPECT_FALSE(cmdline_args.contains("-nonexistent"));
    }

    TEST_F(CmdLineArgsTest, FindParamIndex)
    {
        constexpr std::array<const char*, 3> argv = {"program", "-f", "file.txt"};
        EXPECT_EQ(model::CmdLineArgs::find_param_index(static_cast<int>(argv.size()), argv.data(), "-f"), 1);
    }

    TEST_F(CmdLineArgsTest, FindParamIndexNoArguments)
    {
        constexpr std::array<const char*, 1> argv = {"program"};
        EXPECT_EQ(model::CmdLineArgs::find_param_index(static_cast<int>(argv.size()), argv.data(), "-f"), -1);
    }

    TEST_F(CmdLineArgsTest, FindParamIndexNonExistentParam)
    {
        constexpr std::array<const char*, 3> argv = {"program", "-f", "file.txt"};
        EXPECT_EQ(model::CmdLineArgs::find_param_index(static_cast<int>(argv.size()), argv.data(), "-nonexistent"), -1);
    }

    TEST_F(CmdLineArgsTest, GetParamOption)
    {
        constexpr std::array<const char*, 3> argv = {"program", "-f", "file.txt"};
        auto option = model::CmdLineArgs::get_param_option(static_cast<int>(argv.size()), argv.data(), "-f");
        ASSERT_TRUE(option.has_value());
        EXPECT_EQ(option.value(), "file.txt");

        option = cmdline_args.get_param_option("game");
        ASSERT_TRUE(option.has_value());
        EXPECT_EQ(option.value(), "cstrike");
    }

    TEST_F(CmdLineArgsTest, GetParamOptionNoOption)
    {
        constexpr std::array<const char*, 2> argv = {"program", "-f"};
        auto option = model::CmdLineArgs::get_param_option(static_cast<int>(argv.size()), argv.data(), "-f");
        EXPECT_FALSE(option.has_value());

        option = cmdline_args.get_param_option("+tos");
        EXPECT_FALSE(option.has_value());
    }

    TEST_F(CmdLineArgsTest, GetParamOptionNoArguments)
    {
        constexpr std::array<const char*, 1> argv = {"program"};
        const auto option = model::CmdLineArgs::get_param_option(static_cast<int>(argv.size()), argv.data(), "-f");
        EXPECT_FALSE(option.has_value());
    }

    TEST_F(CmdLineArgsTest, GetParamOptionNonExistentParam)
    {
        constexpr std::array<const char*, 3> argv = {"program", "-f", "file.txt"};
        auto option = model::CmdLineArgs::get_param_option(static_cast<int>(argv.size()), argv.data(), "-nonexistent");
        EXPECT_FALSE(option.has_value());

        option = cmdline_args.get_param_option("-nonexistent");
        EXPECT_FALSE(option.has_value());
    }

    TEST_F(CmdLineArgsTest, GetParamOptionAsInt)
    {
        const auto option = cmdline_args.get_param_option_as<int>("maxplayers");
        ASSERT_TRUE(option.has_value());
        EXPECT_EQ(option.value(), 32);
    }

    TEST_F(CmdLineArgsTest, GetParamOptionAsNoOption)
    {
        const auto option = cmdline_args.get_param_option_as<int>("tos");
        EXPECT_FALSE(option.has_value());
    }

    TEST_F(CmdLineArgsTest, GetParamOptionAsInvalidType)
    {
        const auto option = cmdline_args.get_param_option_as<int>("game");
        EXPECT_FALSE(option.has_value());
    }

    TEST_F(CmdLineArgsTest, GetParamOptionAsNonExistentParam)
    {
        const auto option = cmdline_args.get_param_option_as<int>("nonexistent");
        EXPECT_FALSE(option.has_value());
    }

    TEST_F(CmdLineArgsTest, LoadParamsFromFile)
    {
        std::ofstream hlds_params1{"hlds_params1"};
        hlds_params1 << "+maxplayers 31\r\n@@hlds_params2\n+map\t \nde_dust" << '\n';
        hlds_params1.close();

        std::ofstream hlds_params2{"@hlds_params2"};
        hlds_params2 << "@@hlds_params3 -num_edicts 2048" << '\n';
        hlds_params2.close();

        std::ofstream hlds_params3{"@hlds_params3"};
        hlds_params3.close();

        constexpr std::array<const char*, 7> argv = {"hlds.exe",      "-game",       "cstrike", "-tos",
                                                     "@hlds_params1", "+maxplayers", "32"};

        const model::CmdLineArgs cmd_args{static_cast<int>(argv.size()), argv.data()};

        EXPECT_STREQ("-game cstrike -tos +maxplayers 31 -num_edicts 2048 +map de_dust +maxplayers 32",
                     cmd_args.get_current());

        std::filesystem::remove("hlds_params1");
        std::filesystem::remove("@hlds_params2");
        std::filesystem::remove("@hlds_params3");
    }
}
