#include "gtest/gtest.h"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

TEST(positional_args_test, test_no_positional_no_arguments)
{
    argument_table args("appname", {});

    parser parser;

    // no exceptions should be thrown
    parser.parse(args);
}

TEST(positional_args_test, test_no_positional_extra_values)
{
    argument_table args("appname", { "argument1" });

    parser parser;

    ASSERT_THROW(parser.parse(args), parser_error);
}

TEST(positional_args_test, test_no_positional_with_switch_extra_values)
{
    argument_table args("appname", { "-v", "argument1" });

    parser parser;
    parser.add_switch({ "-v" });

    ASSERT_THROW(parser.parse(args), parser_error);
}

TEST(positional_args_test, test_allowed_no_arguments)
{
    argument_table args("appname", {});

    parser parser;
    parser.add_positional("values").set_max_count_unlimited();

    auto results = parser.parse(args);

    ASSERT_EQ(static_cast<std::size_t>(0), results.get_count("values"));
}

TEST(positional_args_test, test_allowed_single_argument)
{
    argument_table args("appname", { "argument1" });

    parser parser;
    parser.add_positional("values").set_max_count_unlimited();

    auto results = parser.parse(args);

    ASSERT_EQ(static_cast<std::size_t>(1), results.get_count("values"));
    ASSERT_EQ(std::string("argument1"), results.get_values("values")[0]);
}

TEST(positional_args_test, test_allowed_multi_argument)
{
    argument_table args("appname", { "argument1", "argument2", "argument3" });

    parser parser;
    parser.add_positional("values").set_max_count_unlimited();

    auto results = parser.parse(args);

    ASSERT_EQ(static_cast<std::size_t>(3), results.get_count("values"));
    ASSERT_EQ(std::string("argument1"), results.get_values("values")[0]);
    ASSERT_EQ(std::string("argument2"), results.get_values("values")[1]);
    ASSERT_EQ(std::string("argument3"), results.get_values("values")[2]);
}

TEST(positional_args_test, test_allowed_no_arguments_with_switch)
{
    argument_table args("appname", { "-v" });

    parser parser;
    parser.add_switch({ "-v" });
    parser.add_positional("values").set_max_count_unlimited();

    auto results = parser.parse(args);

    ASSERT_EQ(static_cast<std::size_t>(0), results.get_count("values"));
}

TEST(positional_args_test, test_allowed_single_argument_with_switch)
{
    argument_table args("appname", { "-v", "argument1" });

    parser parser;
    parser.add_switch({ "-v" });
    parser.add_positional("values").set_max_count_unlimited();

    auto results = parser.parse(args);

    ASSERT_EQ(static_cast<std::size_t>(1), results.get_count("values"));
    ASSERT_EQ(std::string("argument1"), results.get_values("values")[0]);
}

TEST(positional_args_test, test_allowed_multi_argument_with_switch)
{
    argument_table args("appname", { "-v", "argument1", "argument2", "argument3" });

    parser parser;
    parser.add_switch({ "-v" });
    parser.add_positional("values").set_max_count_unlimited();

    auto results = parser.parse(args);

    ASSERT_EQ(static_cast<std::size_t>(3), results.get_count("values"));
    ASSERT_EQ(std::string("argument1"), results.get_values("values")[0]);
    ASSERT_EQ(std::string("argument2"), results.get_values("values")[1]);
    ASSERT_EQ(std::string("argument3"), results.get_values("values")[2]);
}

TEST(positional_args_test, test_required_bad_config)
{
    argument_table args("appname", { "-v", "argument1", "argument2", "argument3" });

    parser parser1;
    // second argument "required" but first "unlimited"
    parser1.add_positional("values1").set_max_count_unlimited();
    parser1.add_positional("values2").set_min_count(1).set_max_count_unlimited();
    ASSERT_THROW(parser1.parse(args), parser_error);
}

TEST(positional_args_test, test_multiple_required_last_required_singlevalue)
{
    argument_table args_not_enough("appname", { "argument1", "argument2" });
    argument_table args_enough("appname", { "argument1", "argument2", "argument3" });
    argument_table args_too_much("appname", { "argument1", "argument2", "argument3", "argument4" });

    parser parser;
    parser.add_positional("arg1").set_min_count(1);
    parser.add_positional("arg2").set_min_count(1);
    parser.add_positional("arg3").set_min_count(1);

    ASSERT_THROW(parser.parse(args_not_enough), parser_error);

    auto results = parser.parse(args_enough);

    ASSERT_EQ(static_cast<std::size_t>(1), results.get_count("arg1"));
    ASSERT_EQ(static_cast<std::size_t>(1), results.get_count("arg2"));
    ASSERT_EQ(static_cast<std::size_t>(1), results.get_count("arg3"));
    ASSERT_EQ(std::string("argument1"), results.get_values("arg1")[0]);
    ASSERT_EQ(std::string("argument2"), results.get_values("arg2")[0]);
    ASSERT_EQ(std::string("argument3"), results.get_values("arg3")[0]);

    ASSERT_THROW(parser.parse(args_too_much), parser_error);
}

TEST(positional_args_test, test_multiple_required_last_optional_singlevalue)
{
    argument_table args_empty("appname", { "argument1", "argument2" });
    argument_table args_single("appname", { "argument1", "argument2", "argument3" });
    argument_table args_too_much("appname", { "argument1", "argument2", "argument3", "argument4" });

    parser parser;
    parser.add_positional("arg1").set_min_count(1);
    parser.add_positional("arg2").set_min_count(1);
    parser.add_positional("arg3");

    auto results_empty = parser.parse(args_empty);

    ASSERT_EQ(static_cast<std::size_t>(1), results_empty.get_count("arg1"));
    ASSERT_EQ(static_cast<std::size_t>(1), results_empty.get_count("arg2"));
    ASSERT_EQ(static_cast<std::size_t>(0), results_empty.get_count("arg3"));
    ASSERT_EQ(std::string("argument1"), results_empty.get_values("arg1")[0]);
    ASSERT_EQ(std::string("argument2"), results_empty.get_values("arg2")[0]);

    auto results_single = parser.parse(args_single);

    ASSERT_EQ(static_cast<std::size_t>(1), results_single.get_count("arg1"));
    ASSERT_EQ(static_cast<std::size_t>(1), results_single.get_count("arg2"));
    ASSERT_EQ(static_cast<std::size_t>(1), results_single.get_count("arg3"));
    ASSERT_EQ(std::string("argument1"), results_single.get_values("arg1")[0]);
    ASSERT_EQ(std::string("argument2"), results_single.get_values("arg2")[0]);
    ASSERT_EQ(std::string("argument3"), results_single.get_values("arg3")[0]);

    ASSERT_THROW(parser.parse(args_too_much), parser_error);
}

TEST(positional_args_test, test_multiple_required_last_required_multivalue)
{
    argument_table args_not_enough("appname", { "argument1", "argument2" });
    argument_table args_enough("appname", { "argument1", "argument2", "argument3" });
    argument_table args_last_multi("appname", { "argument1", "argument2", "argument3", "argument4", "argument3" });

    parser parser;
    parser.add_positional("arg1").set_min_count(1);
    parser.add_positional("arg2").set_min_count(1);
    parser.add_positional("arg3").set_min_count(1).set_max_count_unlimited();

    ASSERT_THROW(parser.parse(args_not_enough), parser_error);

    auto results_enough = parser.parse(args_enough);

    ASSERT_EQ(static_cast<std::size_t>(1), results_enough.get_count("arg1"));
    ASSERT_EQ(static_cast<std::size_t>(1), results_enough.get_count("arg2"));
    ASSERT_EQ(static_cast<std::size_t>(1), results_enough.get_count("arg3"));
    ASSERT_EQ(std::string("argument1"), results_enough.get_values("arg1")[0]);
    ASSERT_EQ(std::string("argument2"), results_enough.get_values("arg2")[0]);
    ASSERT_EQ(std::string("argument3"), results_enough.get_values("arg3")[0]);

    auto results_last_multi = parser.parse(args_last_multi);

    ASSERT_EQ(static_cast<std::size_t>(1), results_last_multi.get_count("arg1"));
    ASSERT_EQ(static_cast<std::size_t>(1), results_last_multi.get_count("arg2"));
    ASSERT_EQ(static_cast<std::size_t>(3), results_last_multi.get_count("arg3"));
    ASSERT_EQ(std::string("argument1"), results_last_multi.get_values("arg1")[0]);
    ASSERT_EQ(std::string("argument2"), results_last_multi.get_values("arg2")[0]);
    ASSERT_EQ(std::string("argument3"), results_last_multi.get_values("arg3")[0]);
    ASSERT_EQ(std::string("argument4"), results_last_multi.get_values("arg3")[1]);
    ASSERT_EQ(std::string("argument3"), results_last_multi.get_values("arg3")[2]);
}

TEST(positional_args_test, test_multiple_required_last_optional_multivalue)
{
    argument_table args_last_empty("appname", { "argument1", "argument2" });
    argument_table last_single("appname", { "argument1", "argument2", "argument3" });
    argument_table args_last_multi("appname", { "argument1", "argument2", "argument3", "argument4", "argument3" });

    parser parser;
    parser.add_positional("arg1").set_min_count(1);
    parser.add_positional("arg2").set_min_count(1);
    parser.add_positional("arg3").set_max_count_unlimited();

    auto results_last_empty = parser.parse(args_last_empty);

    ASSERT_EQ(static_cast<std::size_t>(1), results_last_empty.get_count("arg1"));
    ASSERT_EQ(static_cast<std::size_t>(1), results_last_empty.get_count("arg2"));
    ASSERT_EQ(static_cast<std::size_t>(0), results_last_empty.get_count("arg3"));
    ASSERT_EQ(std::string("argument1"), results_last_empty.get_values("arg1")[0]);
    ASSERT_EQ(std::string("argument2"), results_last_empty.get_values("arg2")[0]);

    auto results_last_single = parser.parse(last_single);

    ASSERT_EQ(static_cast<std::size_t>(1), results_last_single.get_count("arg1"));
    ASSERT_EQ(static_cast<std::size_t>(1), results_last_single.get_count("arg2"));
    ASSERT_EQ(static_cast<std::size_t>(1), results_last_single.get_count("arg3"));
    ASSERT_EQ(std::string("argument1"), results_last_single.get_values("arg1")[0]);
    ASSERT_EQ(std::string("argument2"), results_last_single.get_values("arg2")[0]);
    ASSERT_EQ(std::string("argument3"), results_last_single.get_values("arg3")[0]);

    auto results_last_multi = parser.parse(args_last_multi);

    ASSERT_EQ(static_cast<std::size_t>(1), results_last_multi.get_count("arg1"));
    ASSERT_EQ(static_cast<std::size_t>(1), results_last_multi.get_count("arg2"));
    ASSERT_EQ(static_cast<std::size_t>(3), results_last_multi.get_count("arg3"));
    ASSERT_EQ(std::string("argument1"), results_last_multi.get_values("arg1")[0]);
    ASSERT_EQ(std::string("argument2"), results_last_multi.get_values("arg2")[0]);
    ASSERT_EQ(std::string("argument3"), results_last_multi.get_values("arg3")[0]);
    ASSERT_EQ(std::string("argument4"), results_last_multi.get_values("arg3")[1]);
    ASSERT_EQ(std::string("argument3"), results_last_multi.get_values("arg3")[2]);
}

TEST(positional_args_test, test_argument_match_positional_name)
{
    argument_table args1("appname", { "files", "argument1", "argument2" });
    argument_table args2("appname", { "files=111", "argument1", "argument2" });

    parser parser;
    parser.add_positional("files").set_max_count_unlimited();

    auto results1 = parser.parse(args1);

    ASSERT_EQ(static_cast<std::size_t>(3), results1.get_count("files"));
    ASSERT_EQ(std::string("files"), results1.get_values("files")[0]);
    ASSERT_EQ(std::string("argument1"), results1.get_values("files")[1]);
    ASSERT_EQ(std::string("argument2"), results1.get_values("files")[2]);

    auto results2 = parser.parse(args2);

    ASSERT_EQ(static_cast<std::size_t>(3), results2.get_count("files"));
    ASSERT_EQ(std::string("files=111"), results2.get_values("files")[0]);
    ASSERT_EQ(std::string("argument1"), results2.get_values("files")[1]);
    ASSERT_EQ(std::string("argument2"), results2.get_values("files")[2]);
}

TEST(positional_args_test, test_default_values)
{
    argument_table args_empty("appname", {});
    argument_table args_given("appname", { "value1", "value2" });

    parser parser1;
    auto& arg = parser1.add_positional("files").set_max_count_unlimited().set_default_values({ "one", "two" });

    auto results_args_given = parser1.parse(args_given);
    ASSERT_EQ(std::size_t(2), results_args_given.get_count("files"));
    ASSERT_EQ(std::string("value1"), results_args_given.get_values("files")[0]);
    ASSERT_EQ(std::string("value2"), results_args_given.get_values("files")[1]);

    auto results = parser1.parse(args_empty);
    ASSERT_EQ(std::size_t(2), results.get_count("files"));
    ASSERT_EQ(std::string("one"), results.get_values("files")[0]);
    ASSERT_EQ(std::string("two"), results.get_values("files")[1]);

    arg.set_default_values({ "one", "two", "three" });
    results = parser1.parse(args_empty);
    ASSERT_EQ(std::size_t(3), results.get_count("files"));
    ASSERT_EQ(std::string("one"), results.get_values("files")[0]);
    ASSERT_EQ(std::string("two"), results.get_values("files")[1]);
    ASSERT_EQ(std::string("three"), results.get_values("files")[2]);

    arg.set_default_value("oko");
    results = parser1.parse(args_empty);
    ASSERT_EQ(std::size_t(1), results.get_count("files"));
    ASSERT_EQ(std::string("oko"), results.get_values("files")[0]);

    arg.set_default_values({});
    results = parser1.parse(args_empty);
    ASSERT_EQ(std::size_t(0), results.get_count("files"));

    parser parser_single;
    parser_single.add_positional("files").set_default_values({ "one", "two" });
    ASSERT_THROW(parser_single.parse(args_empty), parser_error);
}

TEST(positional_args_test, test_min_max_count)
{
    argument_table args0("appname", { "value1" });
    argument_table args1("appname", { "value1", "value2", "value3" });
    argument_table args2("appname", { "value1", "value2", "value3", "value4", "value5" });

    parser parser;
    auto& arg_patterns = parser.add_positional("patterns").set_min_count(1).set_max_count(2);
    auto& arg_files = parser.add_positional("files").set_max_count(2);

    auto results = parser.parse(args0);
    ASSERT_EQ(std::size_t(1), results.get_count("patterns"));
    ASSERT_EQ(std::string("value1"), results.get_values("patterns")[0]);
    ASSERT_EQ(std::size_t(0), results.get_count("files"));

    results = parser.parse(args1);
    ASSERT_EQ(std::size_t(2), results.get_count("patterns"));
    ASSERT_EQ(std::string("value1"), results.get_values("patterns")[0]);
    ASSERT_EQ(std::string("value2"), results.get_values("patterns")[1]);
    ASSERT_EQ(std::size_t(1), results.get_count("files"));
    ASSERT_EQ(std::string("value3"), results.get_values("files")[0]);

    ASSERT_THROW(parser.parse(args2), parser_error);

    arg_patterns.set_min_count(3).set_max_count(3);
    arg_files.set_min_count(2).set_max_count_unlimited();
    results = parser.parse(args2);
    ASSERT_EQ(std::size_t(3), results.get_count("patterns"));
    ASSERT_EQ(std::string("value1"), results.get_values("patterns")[0]);
    ASSERT_EQ(std::string("value2"), results.get_values("patterns")[1]);
    ASSERT_EQ(std::string("value3"), results.get_values("patterns")[2]);
    ASSERT_EQ(std::size_t(2), results.get_count("files"));
    ASSERT_EQ(std::string("value4"), results.get_values("files")[0]);
    ASSERT_EQ(std::string("value5"), results.get_values("files")[1]);
}

TEST(positional_args_test, test_allowed_values)
{
    parser parser;
    parser.add_positional("value").set_allowed_values({ "a", "b", "c" });

    auto results1 = parser.parse(argument_table("app", { "a" }));
    ASSERT_EQ(std::size_t(1), results1.get_count("value"));
    ASSERT_EQ(std::string("a"), results1.get_values("value")[0]);

    auto results2 = parser.parse(argument_table("app", { "b" }));
    ASSERT_EQ(std::size_t(1), results2.get_count("value"));
    ASSERT_EQ(std::string("b"), results2.get_values("value")[0]);

    auto results3 = parser.parse(argument_table("app", { "c" }));
    ASSERT_EQ(std::size_t(1), results3.get_count("value"));
    ASSERT_EQ(std::string("c"), results3.get_values("value")[0]);

    ASSERT_THROW(parser.parse(argument_table("app", { "d" })), parser_error);
}

} // namespace args
} // namespace oct
