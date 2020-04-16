#include "gtest/gtest.h"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

TEST(switch_args_test, test_happy_hapth)
{
    using dictionary_type = default_parser_dictionary<char>;

    dictionary_type dictionary;

    argument_table args("appname", { "-a", "-c" });

    parser parser;
    parser.add_switch({ "-a" });
    parser.add_switch({ "-b" });
    parser.add_switch({ "-c" });

    auto results = parser.parse(args);
    ASSERT_TRUE(results.has_value("-a"));
    ASSERT_EQ(static_cast<std::size_t>(1), results.get_values("-a").size());
    ASSERT_EQ(dictionary.get_switch_enabled_literal(), results.get_values("-a")[0]);
    ASSERT_TRUE(!results.has_value("-b"));
    ASSERT_EQ(static_cast<std::size_t>(0), results.get_values("-b").size());
    ASSERT_TRUE(results.has_value("-c"));
    ASSERT_EQ(static_cast<std::size_t>(1), results.get_values("-c").size());
    ASSERT_EQ(dictionary.get_switch_enabled_literal(), results.get_values("-c")[0]);
}

TEST(switch_args_test, test_no_names)
{
    parser parser;
    ASSERT_THROW(parser.add_switch({}), invalid_argument_name);
}

TEST(switch_args_test, test_multiple_names)
{
    argument_table args("appname", { "-a", "-d", "--beta" });

    parser parser;
    parser.add_switch({ "-a", "--alpha" });
    parser.add_switch({ "-b", "--beta", "beatrice" });
    parser.add_switch({ "-c", "--charlie" });
    parser.add_switch({ "-d", "--delta" });

    auto results = parser.parse(args);
    ASSERT_TRUE(results.has_value("-a"));
    ASSERT_TRUE(results.has_value("--alpha"));
    ASSERT_TRUE(results.has_value("-b"));
    ASSERT_TRUE(results.has_value("--beta"));
    ASSERT_TRUE(results.has_value("beatrice"));
    ASSERT_TRUE(!results.has_value("-c"));
    ASSERT_TRUE(!results.has_value("--charlie"));
    ASSERT_TRUE(results.has_value("-d"));
    ASSERT_TRUE(results.has_value("--delta"));
}

TEST(switch_args_test, test_invalid_name)
{
    parser parser;
    ASSERT_THROW(parser.add_switch({ "" }), invalid_argument_name);
    ASSERT_THROW(parser.add_switch({ "\t\n\r" }), invalid_argument_name);
    ASSERT_THROW(parser.add_switch({ "a " }), invalid_argument_name);
    ASSERT_THROW(parser.add_switch({ " b" }), invalid_argument_name);
    ASSERT_THROW(parser.add_switch({ " c " }), invalid_argument_name);
    ASSERT_THROW(parser.add_switch({ "=c" }), invalid_argument_name);
    ASSERT_THROW(parser.add_switch({ "c=" }), invalid_argument_name);
    ASSERT_THROW(parser.add_switch({ "c==c" }), invalid_argument_name);
    ASSERT_THROW(parser.add_switch({ "=" }), invalid_argument_name);
}

TEST(switch_args_test, test_duplicated_names)
{
    parser parser;
    ASSERT_THROW(parser.add_switch({ "-h", "-h" }), invalid_argument_name);
    ASSERT_THROW(parser.add_switch({ "-v", "--version", "-v" }), invalid_argument_name);
}

TEST(switch_args_test, test_conflicting_names)
{
    parser parser;
    parser.add_switch({ "-h" });
    ASSERT_THROW(parser.add_switch({ "-h" }), invalid_argument_name);
}

TEST(switch_args_test, test_duplicated_value)
{
    argument_table args("appname", { "-v", "-v" });

    parser parser;
    parser.add_switch({ "-v" });

    ASSERT_THROW(parser.parse(args), parser_error);
}

TEST(switch_args_test, test_multiple_values)
{
    argument_table args1("appname", { "-v" });
    argument_table args2("appname", { "-v", "-v", "-a" });
    argument_table args4("appname", { "-v", "-v", "-v", "-v" });

    parser parser;
    parser.add_switch({ "-v" }).set_max_count(4);
    parser.add_switch({ "-a" }).set_max_count(2);

    auto results1 = parser.parse(args1);
    ASSERT_EQ(static_cast<std::size_t>(1), results1.get_count("-v"));
    ASSERT_EQ(static_cast<std::size_t>(0), results1.get_count("-a"));
    auto results2 = parser.parse(args2);
    ASSERT_EQ(static_cast<std::size_t>(2), results2.get_count("-v"));
    ASSERT_EQ(static_cast<std::size_t>(1), results2.get_count("-a"));
    auto results4 = parser.parse(args4);
    ASSERT_EQ(static_cast<std::size_t>(4), results4.get_count("-v"));
    ASSERT_EQ(static_cast<std::size_t>(0), results4.get_count("-a"));
}

TEST(switch_args_test, test_more_values_than_count)
{
    argument_table args("appname", { "-v", "-v", "-v" });

    parser parser;
    parser.add_switch({ "-v" }).set_max_count(2);

    ASSERT_THROW(parser.parse(args), parser_error);
}

TEST(switch_args_test, test_unlimited_values)
{
    const std::size_t COUNT = 543;
    argument_table args("appname", std::vector<std::string>(COUNT, "--arg"));

    parser parser;
    parser.add_switch({ "--arg" }).set_max_count_unlimited();

    auto results = parser.parse(args);
    ASSERT_EQ(COUNT, results.get_count("--arg"));
}

TEST(switch_args_test, test_default_values)
{
    argument_table args_empty("appname", {});
    argument_table args_one("appname", { "-v" });

    parser parser;
    parser.add_switch({ "-v" }).set_max_count(3);

    auto results_one = parser.parse(args_one);
    ASSERT_EQ(std::size_t(1), results_one.get_count("-v"));

    auto results = parser.parse(args_empty);
    ASSERT_EQ(std::size_t(0), results.get_count("-v"));
}

} // namespace args
} // namespace oct
