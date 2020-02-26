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
    ASSERT_EQ(std::size_t(0), results.count("--help"));
    ASSERT_EQ(std::size_t(0), results.count("--version"));
    ASSERT_EQ(std::size_t(0), results.count("--verbose"));
    ASSERT_EQ(std::size_t(0), results.count("files"));
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
    ASSERT_EQ(std::size_t(0), results.count("--help"));
    ASSERT_EQ(std::size_t(0), results.count("--version"));
    ASSERT_EQ(std::size_t(1), results.count("--verbose"));
    ASSERT_EQ(std::size_t(0), results.count("files"));
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
    ASSERT_EQ(std::size_t(0), results.count("--help"));
    ASSERT_EQ(std::size_t(1), results.count("--version"));
    ASSERT_EQ(std::size_t(0), results.count("--verbose"));
    ASSERT_EQ(std::size_t(0), results.count("files"));
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
    ASSERT_EQ(std::size_t(0), results.count("--help"));
    ASSERT_EQ(std::size_t(0), results.count("--version"));
    ASSERT_EQ(std::size_t(0), results.count("--verbose"));
    ASSERT_EQ(std::size_t(2), results.count("files"));
}

} // namespace args
} // namespace oct
