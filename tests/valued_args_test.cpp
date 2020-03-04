#include "gtest/gtest.h"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

TEST(valued_args_test, test_happy_hapth)
{
    argument_table args("appname", { "-a", "valuea", "-c", "valuec", "-a", "valueb" });

    parser parser;
    parser.add_valued({ "-a" }).set_max_count_unlimited();
    parser.add_valued({ "-b" });
    parser.add_valued({ "-c" });

    auto results = parser.parse(args);

    ASSERT_TRUE(results.has_value("-a"));
    ASSERT_EQ(static_cast<std::size_t>(2), results.values("-a").size());
    ASSERT_EQ(std::string("valuea"), results.values("-a")[0]);
    ASSERT_EQ(std::string("valueb"), results.values("-a")[1]);

    ASSERT_TRUE(!results.has_value("-b"));
    ASSERT_EQ(static_cast<std::size_t>(0), results.values("-b").size());

    ASSERT_TRUE(results.has_value("-c"));
    ASSERT_EQ(static_cast<std::size_t>(1), results.values("-c").size());
    ASSERT_EQ(std::string("valuec"), results.values("-c")[0]);
}

TEST(valued_args_test, test_no_names)
{
    parser parser;
    ASSERT_THROW(parser.add_valued({}), invalid_argument_name);
}

TEST(valued_args_test, test_multiple_names)
{
    argument_table args("appname", { "-a", "val1", "-d", "val2", "--beta", "val3" });

    parser parser;
    parser.add_valued({ "-a", "--alpha" });
    parser.add_valued({ "-b", "--beta", "beatrice" });
    parser.add_valued({ "-c", "--charlie" });
    parser.add_valued({ "-d", "--delta" });

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

TEST(valued_args_test, test_invalid_name)
{
    parser parser;
    ASSERT_THROW(parser.add_valued({ "" }), invalid_argument_name);
    ASSERT_THROW(parser.add_valued({ "\t\n\r" }), invalid_argument_name);
    ASSERT_THROW(parser.add_valued({ "a " }), invalid_argument_name);
    ASSERT_THROW(parser.add_valued({ " b" }), invalid_argument_name);
    ASSERT_THROW(parser.add_valued({ " c " }), invalid_argument_name);
}

TEST(valued_args_test, test_duplicated_names)
{
    parser parser;
    ASSERT_THROW(parser.add_valued({ "-h", "-h" }), invalid_argument_name);
    ASSERT_THROW(parser.add_valued({ "-v", "--version", "-v" }), invalid_argument_name);
}

TEST(valued_args_test, test_conflicting_names)
{
    parser parser;
    parser.add_valued({ "-h" });
    ASSERT_THROW(parser.add_valued({ "-h" }), invalid_argument_name);
}

TEST(valued_args_test, test_duplicated_value)
{
    argument_table args("appname", { "-v", "0", "-v", "1" });

    parser parser;
    parser.add_valued({ "-v" });

    ASSERT_THROW(parser.parse(args), parser_error);
}

TEST(valued_args_test, test_multiple_values)
{
    argument_table args1("appname", { "-v", "0" });
    argument_table args2("appname", { "-v", "0", "-v", "1", "-a", "0" });
    argument_table args4("appname", { "-v", "0", "-v", "2", "-v", "0", "-v", "1" });

    parser parser;
    parser.add_valued({ "-v" }).set_max_count(4);
    parser.add_valued({ "-a" }).set_max_count(2);

    auto results1 = parser.parse(args1);
    ASSERT_EQ(static_cast<std::size_t>(1), results1.count("-v"));
    ASSERT_EQ(static_cast<std::size_t>(0), results1.count("-a"));
    auto results2 = parser.parse(args2);
    ASSERT_EQ(static_cast<std::size_t>(2), results2.count("-v"));
    ASSERT_EQ(static_cast<std::size_t>(1), results2.count("-a"));
    auto results4 = parser.parse(args4);
    ASSERT_EQ(static_cast<std::size_t>(4), results4.count("-v"));
    ASSERT_EQ(static_cast<std::size_t>(0), results4.count("-a"));
}

TEST(valued_args_test, test_more_values_than_count)
{
    argument_table args("appname", { "-v", "1", "-v", "2", "-v", "3" });

    parser parser;
    parser.add_valued({ "-v" }).set_max_count(2);

    ASSERT_THROW(parser.parse(args), parser_error);
}

TEST(valued_args_test, test_unlimited_values)
{
    const std::size_t COUNT = 543;
    argument_table args("appname", std::vector<std::string>(COUNT, "--arg=3"));

    parser parser;
    parser.add_valued({ "--arg" }).set_max_count_unlimited();

    auto results = parser.parse(args);
    ASSERT_EQ(COUNT, results.count("--arg"));
}

TEST(valued_args_test, test_missing_value)
{
    argument_table args1("appname", { "-v" });
    argument_table args2("appname", { "-a", "1", "-v" });

    parser parser;
    parser.add_valued({ "-a" });
    parser.add_valued({ "-v" });

    ASSERT_THROW(parser.parse(args1), parser_error);
    ASSERT_THROW(parser.parse(args2), parser_error);
}

TEST(valued_args_test, test_value_is_arg_name)
{
    argument_table args("appname", { "-v", "-a", "-a", "-v" });

    parser parser;
    parser.add_valued({ "-a" });
    parser.add_valued({ "-v" });

    auto results = parser.parse(args);

    ASSERT_TRUE(results.has_value("-a"));
    ASSERT_EQ(static_cast<std::size_t>(1), results.values("-a").size());
    ASSERT_EQ(std::string("-v"), results.values("-a")[0]);

    ASSERT_TRUE(results.has_value("-v"));
    ASSERT_EQ(static_cast<std::size_t>(1), results.values("-v").size());
    ASSERT_EQ(std::string("-a"), results.values("-v")[0]);
}

TEST(valued_args_test, test_switch_valarg_name_conflict)
{
    parser parser;
    parser.add_switch({ "-s", "--switch" });
    parser.add_valued({ "-v", "--value" });
    ASSERT_THROW(parser.add_valued({ "-s" }), invalid_argument_name);
    ASSERT_THROW(parser.add_valued({ "--switch" }), invalid_argument_name);
    ASSERT_THROW(parser.add_switch({ "-v" }), invalid_argument_name);
    ASSERT_THROW(parser.add_switch({ "--value" }), invalid_argument_name);
}

TEST(valued_args_test, test_equal_mode_value_valid)
{
    argument_table args1("appname", { "-v=0" });
    argument_table args2("appname", { "-v=value" });

    parser parser;
    parser.add_valued({ "-v" });

    auto results1 = parser.parse(args1);
    ASSERT_EQ(static_cast<std::size_t>(1), results1.count("-v"));
    ASSERT_EQ(std::string("0"), results1.values("-v")[0]);

    auto results2 = parser.parse(args2);
    ASSERT_EQ(static_cast<std::size_t>(1), results2.count("-v"));
    ASSERT_EQ(std::string("value"), results2.values("-v")[0]);
}

TEST(valued_args_test, test_equal_mode_value_mixed)
{
    argument_table args1("appname", { "-v=0", "-v", "1" });
    argument_table args2("appname", { "-v=value1", "-a", "-v", "value2", "-v=value3" });

    parser parser;
    parser.add_valued({ "-v" }).set_max_count_unlimited();
    parser.add_switch({ "-a" });

    auto results1 = parser.parse(args1);
    ASSERT_EQ(static_cast<std::size_t>(2), results1.count("-v"));
    ASSERT_EQ(std::string("0"), results1.values("-v")[0]);
    ASSERT_EQ(std::string("1"), results1.values("-v")[1]);

    auto results2 = parser.parse(args2);
    ASSERT_EQ(static_cast<std::size_t>(3), results2.count("-v"));
    ASSERT_EQ(std::string("value1"), results2.values("-v")[0]);
    ASSERT_EQ(std::string("value2"), results2.values("-v")[1]);
    ASSERT_EQ(std::string("value3"), results2.values("-v")[2]);
}

TEST(valued_args_test, test_equal_value_invalid)
{
    argument_table args1("appname", { "=0" });
    argument_table args2("appname", { "-a=1" });
    argument_table args3("appname", { "-s=xyz" });

    parser parser;
    parser.add_valued({ "-v" });
    parser.add_switch({ "-s" });

    ASSERT_THROW(parser.parse(args1), parser_error);
    ASSERT_THROW(parser.parse(args2), parser_error);
    ASSERT_THROW(parser.parse(args3), parser_error);
}

TEST(valued_args_test, test_equal_value_with_equal)
{
    argument_table args1("appname", { "-v==0" });
    argument_table args2("appname", { "-v", "=value" });
    argument_table args3("appname", { "-v=a==b" });

    parser parser;
    parser.add_valued({ "-v" });

    auto results1 = parser.parse(args1);
    ASSERT_EQ(static_cast<std::size_t>(1), results1.count("-v"));
    ASSERT_EQ(std::string("=0"), results1.values("-v")[0]);

    auto results2 = parser.parse(args2);
    ASSERT_EQ(static_cast<std::size_t>(1), results2.count("-v"));
    ASSERT_EQ(std::string("=value"), results2.values("-v")[0]);

    auto results3 = parser.parse(args3);
    ASSERT_EQ(static_cast<std::size_t>(1), results3.count("-v"));
    ASSERT_EQ(std::string("a==b"), results3.values("-v")[0]);
}

TEST(valued_args_test, test_default_values)
{
    argument_table args_empty("appname", {});
    argument_table args_one("appname", { "-v", "value" });

    parser parser;
    auto& arg = parser.add_valued({ "-v" }).set_max_count(3).set_default_values({ "one", "two" });

    auto results_one = parser.parse(args_one);
    ASSERT_EQ(std::size_t(1), results_one.count("-v"));
    ASSERT_EQ(std::string("value"), results_one.values("-v")[0]);

    auto results = parser.parse(args_empty);
    ASSERT_EQ(std::size_t(2), results.count("-v"));
    ASSERT_EQ(std::string("one"), results.values("-v")[0]);
    ASSERT_EQ(std::string("two"), results.values("-v")[1]);

    arg.set_default_values({ "one", "two", "three", "four" });
    ASSERT_THROW(parser.parse(args_empty), parser_error);

    arg.set_default_values({ "one", "two", "three" });
    results = parser.parse(args_empty);
    ASSERT_EQ(std::size_t(3), results.count("-v"));
    ASSERT_EQ(std::string("one"), results.values("-v")[0]);
    ASSERT_EQ(std::string("two"), results.values("-v")[1]);
    ASSERT_EQ(std::string("three"), results.values("-v")[2]);

    arg.set_default_value("oko");
    results = parser.parse(args_empty);
    ASSERT_EQ(std::size_t(1), results.count("-v"));
    ASSERT_EQ(std::string("oko"), results.values("-v")[0]);

    arg.set_default_values({});
    results = parser.parse(args_empty);
    ASSERT_EQ(std::size_t(0), results.count("-v"));
}

TEST(valued_args_test, test_min_max_count)
{
    argument_table args("appname", { "-v", "value1", "-v", "value2" });

    parser parser;
    auto& arg = parser.add_valued({ "-v" }).set_max_count_unlimited();

    auto results = parser.parse(args);
    ASSERT_EQ(std::size_t(2), results.count("-v"));

    arg.set_min_count(1).set_max_count(2);
    results = parser.parse(args);
    ASSERT_EQ(std::size_t(2), results.count("-v"));

    arg.set_min_count(2).set_max_count(2);
    results = parser.parse(args);
    ASSERT_EQ(std::size_t(2), results.count("-v"));

    arg.set_min_count(3);
    ASSERT_THROW(parser.parse(args), parser_error);

    arg.set_min_count(2).set_max_count(1);
    ASSERT_THROW(parser.parse(args), parser_error);
}

TEST(valued_args_test, test_allowed_values)
{
    parser parser;
    parser.add_valued({ "-v" }).set_allowed_values({ "a", "b", "c" });

    auto results1 = parser.parse(argument_table("app", { "-v", "a" }));
    ASSERT_EQ(std::size_t(1), results1.count("-v"));
    ASSERT_EQ(std::string("a"), results1.values("-v")[0]);

    auto results2 = parser.parse(argument_table("app", { "-v", "b" }));
    ASSERT_EQ(std::size_t(1), results2.count("-v"));
    ASSERT_EQ(std::string("b"), results2.values("-v")[0]);

    auto results3 = parser.parse(argument_table("app", { "-v", "c" }));
    ASSERT_EQ(std::size_t(1), results3.count("-v"));
    ASSERT_EQ(std::string("c"), results3.values("-v")[0]);

    ASSERT_THROW(parser.parse(argument_table("app", { "-v", "d" })), parser_error);
}

} // namespace args
} // namespace oct
