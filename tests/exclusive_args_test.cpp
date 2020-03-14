#include "gtest/gtest.h"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

TEST(exclusive_args_test, test_not_given_no_args)
{
    argument_table args("appname", {});

    parser parser;
    parser.add_exclusive({ "--help" });
    parser.add_exclusive({ "--version" });
    parser.add_switch({ "--verbose" });
    parser.add_positional("files").set_max_count_unlimited();

    auto results = parser.parse(args);
    ASSERT_EQ(std::size_t(0), results.get_count("--help"));
    ASSERT_EQ(std::size_t(0), results.get_count("--version"));
    ASSERT_EQ(std::size_t(0), results.get_count("--verbose"));
    ASSERT_EQ(std::size_t(0), results.get_count("files"));
}

TEST(exclusive_args_test, test_switch_given)
{
    argument_table args("appname", { "--verbose" });

    parser parser;
    parser.add_exclusive({ "--help" });
    parser.add_exclusive({ "--version" });
    parser.add_switch({ "--verbose" });
    parser.add_positional("files").set_max_count_unlimited();

    auto results = parser.parse(args);
    ASSERT_EQ(std::size_t(0), results.get_count("--help"));
    ASSERT_EQ(std::size_t(0), results.get_count("--version"));
    ASSERT_EQ(std::size_t(1), results.get_count("--verbose"));
    ASSERT_EQ(std::size_t(0), results.get_count("files"));
}

TEST(exclusive_args_test, test_one_given)
{
    argument_table args("appname", { "--version" });

    parser parser;
    parser.add_exclusive({ "--help" });
    parser.add_exclusive({ "--version" });
    parser.add_switch({ "--verbose" });
    parser.add_positional("files").set_max_count_unlimited();

    auto results = parser.parse(args);
    ASSERT_EQ(std::size_t(0), results.get_count("--help"));
    ASSERT_EQ(std::size_t(1), results.get_count("--version"));
    ASSERT_EQ(std::size_t(0), results.get_count("--verbose"));
    ASSERT_EQ(std::size_t(0), results.get_count("files"));
}

TEST(exclusive_args_test, test_two_given)
{
    argument_table args("appname", { "--version", "--help" });

    parser parser;
    parser.add_exclusive({ "--help" });
    parser.add_exclusive({ "--version" });
    parser.add_switch({ "--verbose" });
    parser.add_positional("files").set_max_count_unlimited();

    auto results = parser.parse(args);
    ASSERT_EQ(std::size_t(0), results.get_count("--help"));
    ASSERT_EQ(std::size_t(0), results.get_count("--version"));
    ASSERT_EQ(std::size_t(0), results.get_count("--verbose"));
    ASSERT_EQ(std::size_t(2), results.get_count("files"));
}

TEST(exclusive_args_test, test_subparsers)
{
    parser parser;
    auto& subparsers_arg = parser.add_subparsers("command");
    parser.add_exclusive({ "--help" });
    parser.add_switch({ "--verbose" });
    auto parser_add = subparsers_arg.add_parser("add");
    parser_add.add_exclusive({ "--help" });
    parser_add.add_switch({ "--verbose" });
    auto parser_sub = subparsers_arg.add_parser("sub");
    parser_sub.add_exclusive({ "--help" });
    parser_sub.add_switch({ "--verbose" });

    auto results_ok_main = parser.parse(argument_table("appname", { "--help" }));
    ASSERT_EQ(std::size_t(1), results_ok_main.get_count("--help"));
    ASSERT_EQ(std::size_t(0), results_ok_main.get_count("--verbose"));
    ASSERT_EQ(std::size_t(0), results_ok_main.get_count("add --help"));
    ASSERT_EQ(std::size_t(0), results_ok_main.get_count("add --verbose"));
    ASSERT_EQ(std::size_t(0), results_ok_main.get_count("sub --help"));
    ASSERT_EQ(std::size_t(0), results_ok_main.get_count("sub --verbose"));

    auto results_ok_add = parser.parse(argument_table("appname", { "add", "--help" }));
    ASSERT_EQ(std::size_t(0), results_ok_add.get_count("--help"));
    ASSERT_EQ(std::size_t(0), results_ok_add.get_count("--verbose"));
    ASSERT_EQ(std::size_t(1), results_ok_add.get_count("add --help"));
    ASSERT_EQ(std::size_t(0), results_ok_add.get_count("add --verbose"));
    ASSERT_EQ(std::size_t(0), results_ok_add.get_count("sub --help"));
    ASSERT_EQ(std::size_t(0), results_ok_add.get_count("sub --verbose"));

    auto results_ok_sub = parser.parse(argument_table("appname", { "sub", "--help" }));
    ASSERT_EQ(std::size_t(0), results_ok_sub.get_count("--help"));
    ASSERT_EQ(std::size_t(0), results_ok_sub.get_count("--verbose"));
    ASSERT_EQ(std::size_t(0), results_ok_sub.get_count("add --help"));
    ASSERT_EQ(std::size_t(0), results_ok_sub.get_count("add --verbose"));
    ASSERT_EQ(std::size_t(1), results_ok_sub.get_count("sub --help"));
    ASSERT_EQ(std::size_t(0), results_ok_sub.get_count("sub --verbose"));

    EXPECT_THROW(parser.parse(argument_table("appname", { "--help", "--verbose" })), parser_error);
    EXPECT_THROW(parser.parse(argument_table("appname", { "--help", "add" })), parser_error);
    EXPECT_THROW(parser.parse(argument_table("appname", { "--help", "add", "--help" })), parser_error);
    EXPECT_THROW(parser.parse(argument_table("appname", { "--verbose", "add", "--help" })), parser_error);
    EXPECT_THROW(parser.parse(argument_table("appname", { "--verbose", "sub", "--help" })), parser_error);
    EXPECT_THROW(parser.parse(argument_table("appname", { "--help", "add", "--help" })), parser_error);
    EXPECT_THROW(parser.parse(argument_table("appname", { "--help", "sub", "--help" })), parser_error);
    EXPECT_THROW(parser.parse(argument_table("appname", { "--help", "add", "--verbose" })), parser_error);
    EXPECT_THROW(parser.parse(argument_table("appname", { "--help", "sub", "--verbose" })), parser_error);
    EXPECT_THROW(parser.parse(argument_table("appname", { "add", "--help", "--verbose" })), parser_error);
    EXPECT_THROW(parser.parse(argument_table("appname", { "sub", "--help", "--verbose" })), parser_error);

    // TODO
}

} // namespace args
} // namespace oct
