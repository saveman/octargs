#include "test_fixture.hpp"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

class basic_parse_test : public test_fixture
{
private:
    void test_argc_argv_parse()
    {
        char app[] { "appname" };
        char arg1[] { "-v" };
        char arg2[] { "argument1" };
        char arg3[] { "argument2" };
        char* argv[] = { app, arg1, arg2, arg3, nullptr };
        int argc = 4;

        parser parser;
        parser.add_switch({ "-v" });
        parser.add_switch({ "--help" });
        parser.set_positional_arguments_enabled(true);

        auto results = parser.parse(argc, argv);
        CPPUNIT_ASSERT(results.has_value("-v"));
        CPPUNIT_ASSERT(!results.has_value("--help"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(2), results.get_positional_arguments().size());
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results.get_positional_arguments()[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results.get_positional_arguments()[1]);
    }

    void test_argument_table_parse()
    {
        argument_table args("appname", { "-v", "argument1", "argument2" });

        parser parser;
        parser.add_switch({ "-v" });
        parser.add_switch({ "--help" });
        parser.set_positional_arguments_enabled(true);

        auto results = parser.parse(args);
        CPPUNIT_ASSERT(results.has_value("-v"));
        CPPUNIT_ASSERT(!results.has_value("--help"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(2), results.get_positional_arguments().size());
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results.get_positional_arguments()[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results.get_positional_arguments()[1]);
    }

    void test_invalid_results_arg()
    {
        argument_table args("appname", { "-v" });

        parser parser;
        parser.add_switch({ "-v" });

        auto results = parser.parse(args);
        CPPUNIT_ASSERT(results.has_value("-v"));
        CPPUNIT_ASSERT_THROW(results.has_value("-a"), logic_error_exception);
    }

    void test_unknown_argument()
    {
        argument_table args("appname", { "-v", "-a" });

        parser parser;
        parser.add_switch({ "-v" });

        CPPUNIT_ASSERT_THROW(parser.parse(args), parse_exception);
    }

    void test_modify_locked_parser()
    {
        argument_table args("appname", { "-v" });

        parser parser;
        parser.add_switch({ "-v" });
        parser.parse(args);
        CPPUNIT_ASSERT_THROW(parser.add_switch({ "-a" }), parser_state_exception);
    }

    CPPUNIT_TEST_SUITE(basic_parse_test);
    CPPUNIT_TEST(test_argc_argv_parse);
    CPPUNIT_TEST(test_argument_table_parse);
    CPPUNIT_TEST(test_invalid_results_arg);
    CPPUNIT_TEST(test_unknown_argument);
    CPPUNIT_TEST(test_modify_locked_parser);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(basic_parse_test);

} // namespace args
} // namespace oct
