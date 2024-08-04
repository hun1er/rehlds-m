/*
 * Half Life 1 SDK License
 * Copyright(c) Valve Corp
 *
 * DISCLAIMER OF WARRANTIES. THE HALF LIFE 1 SDK AND ANY OTHER MATERIAL
 * DOWNLOADED BY LICENSEE IS PROVIDED "AS IS". VALVE AND ITS SUPPLIERS
 * DISCLAIM ALL WARRANTIES WITH RESPECT TO THE SDK, EITHER EXPRESS OR IMPLIED,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY,
 * NON-INFRINGEMENT, TITLE AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * LIMITATION OF LIABILITY. IN NO EVENT SHALL VALVE OR ITS SUPPLIERS BE LIABLE
 * FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER
 * (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
 * BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY
 * LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THE ENGINE AND/OR THE
 * SDK, EVEN IF VALVE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * For commercial use, contact: sourceengine@valvesoftware.com
 */

#include "core/cmdline_args.hpp"
#include <gtest/gtest.h>
#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>

namespace {
    class CmdLineArgsTest : public ::testing::Test {
      public:
        CmdLineArgsTest() : cmdline_args(Core::CmdLineArgs::from_command_line(ARGC, CmdLineArgsTest::ARGV.data()))
        {
        }

      protected:
        static constexpr std::size_t ARGC = 10;

        static constexpr std::array<const char*, ARGC> ARGV = {
          "hlds.exe", "-game", "cstrike", "-tos", "-pingboost", "2", "+maxplayers", "32", "+map", "de_dust2"};

        Core::CmdLineArgs cmdline_args;
    };

    TEST_F(CmdLineArgsTest, GetCurrent)
    {
        EXPECT_STREQ(cmdline_args.get_current(), "-game cstrike -tos -pingboost 2 +maxplayers 32 +map de_dust2");
    }

    TEST_F(CmdLineArgsTest, GetCurrentEmpty)
    {
        constexpr std::array<const char*, 0> argv{};
        const auto args = Core::CmdLineArgs::from_command_line(argv.size(), argv.data());
        EXPECT_STREQ(args.get_current(), "");
    }

    TEST_F(CmdLineArgsTest, PushFront)
    {
        cmdline_args.push_front("-newparam", "newoption");
        EXPECT_STREQ(
          cmdline_args.get_current(),
          "-newparam newoption -game cstrike -tos -pingboost 2 +maxplayers 32 +map de_dust2");
    }

    TEST_F(CmdLineArgsTest, PushFrontNoOption)
    {
        cmdline_args.push_front("-newparam");
        EXPECT_STREQ(
          cmdline_args.get_current(), "-newparam -game cstrike -tos -pingboost 2 +maxplayers 32 +map de_dust2");
    }

    TEST_F(CmdLineArgsTest, PushFrontExistingParam)
    {
        cmdline_args.push_front("+pingboost", "5");
        EXPECT_STREQ(cmdline_args.get_current(), "+pingboost 5 -game cstrike -tos +maxplayers 32 +map de_dust2");
    }

    TEST_F(CmdLineArgsTest, PushBack)
    {
        cmdline_args.push_back("-newparam", "newoption");
        EXPECT_STREQ(
          cmdline_args.get_current(),
          "-game cstrike -tos -pingboost 2 +maxplayers 32 +map de_dust2 -newparam newoption");
    }

    TEST_F(CmdLineArgsTest, PushBackNoOption)
    {
        cmdline_args.push_back("-newparam");
        EXPECT_STREQ(
          cmdline_args.get_current(), "-game cstrike -tos -pingboost 2 +maxplayers 32 +map de_dust2 -newparam");
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

    TEST_F(CmdLineArgsTest, GetParamOption)
    {
        const auto option = cmdline_args.get_argument_option("game");
        ASSERT_TRUE(option.has_value());
        EXPECT_EQ(option.value(), "cstrike");
    }

    TEST_F(CmdLineArgsTest, GetParamOptionNoOption)
    {
        const auto option = cmdline_args.get_argument_option("+tos");
        EXPECT_FALSE(option.has_value());
    }

    TEST_F(CmdLineArgsTest, GetParamOptionNonExistentParam)
    {
        const auto option = cmdline_args.get_argument_option("-nonexistent");
        EXPECT_FALSE(option.has_value());
    }

    TEST_F(CmdLineArgsTest, GetParamOptionAsInt)
    {
        const auto option = cmdline_args.get_argument_option_as<int>("maxplayers");
        ASSERT_TRUE(option.has_value());
        EXPECT_EQ(option.value(), 32);
    }

    TEST_F(CmdLineArgsTest, GetParamOptionAsNoOption)
    {
        const auto option = cmdline_args.get_argument_option_as<int>("tos");
        EXPECT_FALSE(option.has_value());
    }

    TEST_F(CmdLineArgsTest, GetParamOptionAsInvalidType)
    {
        const auto option = cmdline_args.get_argument_option_as<int>("game");
        EXPECT_FALSE(option.has_value());
    }

    TEST_F(CmdLineArgsTest, GetParamOptionAsNonExistentParam)
    {
        const auto option = cmdline_args.get_argument_option_as<int>("nonexistent");
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

        const auto cmd_args = Core::CmdLineArgs::from_command_line(argv.size(), argv.data());

        EXPECT_STREQ(
          "-game cstrike -tos +maxplayers 31 -num_edicts 2048 +map de_dust +maxplayers 32", cmd_args.get_current());

        std::filesystem::remove("hlds_params1");
        std::filesystem::remove("@hlds_params2");
        std::filesystem::remove("@hlds_params3");
    }
}
