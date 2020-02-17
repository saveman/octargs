#include "test_fixture.hpp"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

class positional_args_test : public test_fixture
{
private:
    void test_no_positional_no_arguments()
    {
        argument_table args("appname", {});

        parser parser;

        // no exceptions should be thrown
        parser.parse(args);
    }

    void test_no_positional_extra_values()
    {
        argument_table args("appname", { "argument1" });

        parser parser;

        CPPUNIT_ASSERT_THROW(parser.parse(args), exception);
    }

    void test_no_positional_with_switch_extra_values()
    {
        argument_table args("appname", { "-v", "argument1" });

        parser parser;
        parser.add_switch({ "-v" });

        CPPUNIT_ASSERT_THROW(parser.parse(args), exception);
    }

    void test_allowed_no_arguments()
    {
        argument_table args("appname", {});

        parser parser;
        parser.add_positional("values", false, true);

        auto results = parser.parse(args);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(0), results.count("values"));
    }

    void test_allowed_single_argument()
    {
        argument_table args("appname", { "argument1" });

        parser parser;
        parser.add_positional("values", false, true);

        auto results = parser.parse(args);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results.count("values"));
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results.values("values")[0]);
    }

    void test_allowed_multi_argument()
    {
        argument_table args("appname", { "argument1", "argument2", "argument3" });

        parser parser;
        parser.add_positional("values", false, true);

        auto results = parser.parse(args);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(3), results.count("values"));
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results.values("values")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results.values("values")[1]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument3"), results.values("values")[2]);
    }

    void test_allowed_no_arguments_with_switch()
    {
        argument_table args("appname", { "-v" });

        parser parser;
        parser.add_switch({ "-v" });
        parser.add_positional("values", false, true);

        auto results = parser.parse(args);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(0), results.count("values"));
    }

    void test_allowed_single_argument_with_switch()
    {
        argument_table args("appname", { "-v", "argument1" });

        parser parser;
        parser.add_switch({ "-v" });
        parser.add_positional("values", false, true);

        auto results = parser.parse(args);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results.count("values"));
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results.values("values")[0]);
    }

    void test_allowed_multi_argument_with_switch()
    {
        argument_table args("appname", { "-v", "argument1", "argument2", "argument3" });

        parser parser;
        parser.add_switch({ "-v" });
        parser.add_positional("values", false, true);

        auto results = parser.parse(args);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(3), results.count("values"));
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results.values("values")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results.values("values")[1]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument3"), results.values("values")[2]);
    }

    void test_required_bad_config()
    {
        argument_table args("appname", { "-v", "argument1", "argument2", "argument3" });

        // first multivalue, so no more can be registered
        parser parser1;
        parser1.add_positional("values1", false, true);
        CPPUNIT_ASSERT_THROW(parser1.add_positional("values2", false, true), configuration_exception);

        // first multivalue, so no more can be registered
        parser parser2;
        parser2.add_positional("values1", true, true);
        CPPUNIT_ASSERT_THROW(parser2.add_positional("values2", true, true), configuration_exception);

        // first multivalue, so no more can be registered
        parser parser3;
        parser3.add_positional("values1", true, true);
        CPPUNIT_ASSERT_THROW(parser3.add_positional("values2", true, false), configuration_exception);

        // first optional, so no more can be registered
        parser parser4;
        parser4.add_positional("values1", false, false);
        CPPUNIT_ASSERT_THROW(parser4.add_positional("values2", true, true), configuration_exception);
    }

    void test_multiple_required_last_required_singlevalue()
    {
        argument_table args_not_enough("appname", { "argument1", "argument2" });
        argument_table args_enough("appname", { "argument1", "argument2", "argument3" });
        argument_table args_too_much("appname", { "argument1", "argument2", "argument3", "argument4" });

        parser parser;
        parser.add_positional("arg1", true, false);
        parser.add_positional("arg2", true, false);
        parser.add_positional("arg3", true, false);

        CPPUNIT_ASSERT_THROW(parser.parse(args_not_enough), parse_exception);

        auto results = parser.parse(args_enough);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results.count("arg1"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results.count("arg2"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results.count("arg3"));
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results.values("arg1")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results.values("arg2")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument3"), results.values("arg3")[0]);

        CPPUNIT_ASSERT_THROW(parser.parse(args_too_much), parse_exception);
    }

    void test_multiple_required_last_optional_singlevalue()
    {
        argument_table args_empty("appname", { "argument1", "argument2" });
        argument_table args_single("appname", { "argument1", "argument2", "argument3" });
        argument_table args_too_much("appname", { "argument1", "argument2", "argument3", "argument4" });

        parser parser;
        parser.add_positional("arg1", true, false);
        parser.add_positional("arg2", true, false);
        parser.add_positional("arg3", false, false);

        auto results_empty = parser.parse(args_empty);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_empty.count("arg1"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_empty.count("arg2"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(0), results_empty.count("arg3"));
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results_empty.values("arg1")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results_empty.values("arg2")[0]);

        auto results_single = parser.parse(args_single);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_single.count("arg1"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_single.count("arg2"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_single.count("arg3"));
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results_single.values("arg1")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results_single.values("arg2")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument3"), results_single.values("arg3")[0]);

        CPPUNIT_ASSERT_THROW(parser.parse(args_too_much), parse_exception);
    }

    void test_multiple_required_last_required_multivalue()
    {
        argument_table args_not_enough("appname", { "argument1", "argument2" });
        argument_table args_enough("appname", { "argument1", "argument2", "argument3" });
        argument_table args_last_multi("appname", { "argument1", "argument2", "argument3", "argument4", "argument3" });

        parser parser;
        parser.add_positional("arg1", true, false);
        parser.add_positional("arg2", true, false);
        parser.add_positional("arg3", true, true);

        CPPUNIT_ASSERT_THROW(parser.parse(args_not_enough), parse_exception);

        auto results_enough = parser.parse(args_enough);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_enough.count("arg1"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_enough.count("arg2"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_enough.count("arg3"));
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results_enough.values("arg1")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results_enough.values("arg2")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument3"), results_enough.values("arg3")[0]);

        auto results_last_multi = parser.parse(args_last_multi);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_last_multi.count("arg1"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_last_multi.count("arg2"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(3), results_last_multi.count("arg3"));
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results_last_multi.values("arg1")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results_last_multi.values("arg2")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument3"), results_last_multi.values("arg3")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument4"), results_last_multi.values("arg3")[1]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument3"), results_last_multi.values("arg3")[2]);
    }

    void test_multiple_required_last_optional_multivalue()
    {
        argument_table args_last_empty("appname", { "argument1", "argument2" });
        argument_table last_single("appname", { "argument1", "argument2", "argument3" });
        argument_table args_last_multi("appname", { "argument1", "argument2", "argument3", "argument4", "argument3" });

        parser parser;
        parser.add_positional("arg1", true, false);
        parser.add_positional("arg2", true, false);
        parser.add_positional("arg3", false, true);

        auto results_last_empty = parser.parse(args_last_empty);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_last_empty.count("arg1"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_last_empty.count("arg2"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(0), results_last_empty.count("arg3"));
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results_last_empty.values("arg1")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results_last_empty.values("arg2")[0]);

        auto results_last_single = parser.parse(last_single);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_last_single.count("arg1"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_last_single.count("arg2"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_last_single.count("arg3"));
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results_last_single.values("arg1")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results_last_single.values("arg2")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument3"), results_last_single.values("arg3")[0]);

        auto results_last_multi = parser.parse(args_last_multi);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_last_multi.count("arg1"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results_last_multi.count("arg2"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(3), results_last_multi.count("arg3"));
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results_last_multi.values("arg1")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results_last_multi.values("arg2")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument3"), results_last_multi.values("arg3")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument4"), results_last_multi.values("arg3")[1]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument3"), results_last_multi.values("arg3")[2]);
    }

    void test_argument_match_positional_name()
    {
        argument_table args1("appname", { "files", "argument1", "argument2" });
        argument_table args2("appname", { "files=111", "argument1", "argument2" });

        parser parser;
        parser.add_positional("files", false, true);

        auto results1 = parser.parse(args1);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(3), results1.count("files"));
        CPPUNIT_ASSERT_EQUAL(std::string("files"), results1.values("files")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results1.values("files")[1]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results1.values("files")[2]);

        auto results2 = parser.parse(args2);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(3), results2.count("files"));
        CPPUNIT_ASSERT_EQUAL(std::string("files=111"), results2.values("files")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results2.values("files")[1]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results2.values("files")[2]);
    }

    void test_default_values()
    {
        argument_table args_empty("appname", {});
        argument_table args_given("appname", { "value1", "value2" });

        parser parser1;
        auto& arg = parser1.add_positional("files", false, true).set_default_values({ "one", "two" });

        auto results_args_given = parser1.parse(args_given);
        CPPUNIT_ASSERT_EQUAL(std::size_t(2), results_args_given.count("files"));
        CPPUNIT_ASSERT_EQUAL(std::string("value1"), results_args_given.values("files")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("value2"), results_args_given.values("files")[1]);

        auto results = parser1.parse(args_empty);
        CPPUNIT_ASSERT_EQUAL(std::size_t(2), results.count("files"));
        CPPUNIT_ASSERT_EQUAL(std::string("one"), results.values("files")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("two"), results.values("files")[1]);

        arg.set_default_values({ "one", "two", "three" });
        results = parser1.parse(args_empty);
        CPPUNIT_ASSERT_EQUAL(std::size_t(3), results.count("files"));
        CPPUNIT_ASSERT_EQUAL(std::string("one"), results.values("files")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("two"), results.values("files")[1]);
        CPPUNIT_ASSERT_EQUAL(std::string("three"), results.values("files")[2]);

        arg.set_default_value("oko");
        results = parser1.parse(args_empty);
        CPPUNIT_ASSERT_EQUAL(std::size_t(1), results.count("files"));
        CPPUNIT_ASSERT_EQUAL(std::string("oko"), results.values("files")[0]);

        arg.set_default_values({});
        results = parser1.parse(args_empty);
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), results.count("files"));

        parser parser_single;
        parser_single.add_positional("files", false, false).set_default_values({ "one", "two" });
        CPPUNIT_ASSERT_THROW(parser_single.parse(args_empty), parse_exception);
    }

    CPPUNIT_TEST_SUITE(positional_args_test);
    CPPUNIT_TEST(test_no_positional_no_arguments);
    CPPUNIT_TEST(test_no_positional_extra_values);
    CPPUNIT_TEST(test_no_positional_with_switch_extra_values);
    CPPUNIT_TEST(test_allowed_no_arguments);
    CPPUNIT_TEST(test_allowed_single_argument);
    CPPUNIT_TEST(test_allowed_multi_argument);
    CPPUNIT_TEST(test_allowed_no_arguments_with_switch);
    CPPUNIT_TEST(test_allowed_single_argument_with_switch);
    CPPUNIT_TEST(test_allowed_multi_argument_with_switch);
    CPPUNIT_TEST(test_required_bad_config);
    CPPUNIT_TEST(test_multiple_required_last_required_singlevalue);
    CPPUNIT_TEST(test_multiple_required_last_optional_singlevalue);
    CPPUNIT_TEST(test_multiple_required_last_required_multivalue);
    CPPUNIT_TEST(test_multiple_required_last_optional_multivalue);
    CPPUNIT_TEST(test_argument_match_positional_name);
    CPPUNIT_TEST(test_default_values);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(positional_args_test);

} // namespace args
} // namespace oct
