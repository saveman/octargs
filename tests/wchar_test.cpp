#include "gtest/gtest.h"

#include "../include/octargs/parser.hpp"

namespace oct
{
namespace args
{

namespace
{

using parser_type = basic_parser<wchar_t, void>;

}

TEST(wchar_test, test_basic)
{
    const wchar_t* test_argv[] = {
        L"appname",
        L"-v",
        L"param1",
        L"param2",
        L"param3",
    };
    int test_argc = sizeof(test_argv) / sizeof(test_argv[0]);

    parser_type parser;
    parser.add_switch({ L"-v", L"--version" });
    parser.add_positional(L"values").set_max_count_unlimited();

    auto results = parser.parse(test_argc, test_argv);
    ASSERT_EQ(static_cast<size_t>(3), results.get_count(L"values"));
    ASSERT_TRUE(results.get_values(L"values")[0] == test_argv[2]);
    ASSERT_TRUE(results.get_values(L"values")[1] == test_argv[3]);
    ASSERT_TRUE(results.get_values(L"values")[2] == test_argv[4]);
    ASSERT_TRUE(results.has_value(L"--version"));
}

} // namespace args
} // namespace oct
