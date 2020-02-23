#include "test_fixture.hpp"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

class parser_test : public test_fixture
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
        parser.add_positional("values").set_max_count_unlimited();

        auto results = parser.parse(argc, argv);
        CPPUNIT_ASSERT_EQUAL(std::string("appname"), results.get_app_name());
        CPPUNIT_ASSERT(results.has_value("-v"));
        CPPUNIT_ASSERT(!results.has_value("--help"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(2), results.count("values"));
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results.values("values")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results.values("values")[1]);
    }

    void test_argument_table_parse()
    {
        argument_table args("appname", { "-v", "argument1", "argument2" });

        parser parser;
        parser.add_switch({ "-v" });
        parser.add_switch({ "--help" });
        parser.add_positional("values").set_max_count_unlimited();

        auto results = parser.parse(args);
        CPPUNIT_ASSERT(results.has_value("-v"));
        CPPUNIT_ASSERT(!results.has_value("--help"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(2), results.count("values"));
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results.values("values")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results.values("values")[1]);
    }

    void test_invalid_results_arg()
    {
        argument_table args("appname", { "-v" });

        parser parser;
        parser.add_switch({ "-v" });

        auto results = parser.parse(args);
        CPPUNIT_ASSERT(results.has_value("-v"));
        CPPUNIT_ASSERT_THROW(results.has_value("-a"), unknown_argument);
    }

    void test_unknown_argument()
    {
        argument_table args("appname", { "-v", "-a" });

        parser parser;
        parser.add_switch({ "-v" });

        CPPUNIT_ASSERT_THROW(parser.parse(args), parser_error);
    }

    CPPUNIT_TEST_SUITE(parser_test);
    CPPUNIT_TEST(test_argc_argv_parse);
    CPPUNIT_TEST(test_argument_table_parse);
    CPPUNIT_TEST(test_invalid_results_arg);
    CPPUNIT_TEST(test_unknown_argument);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(parser_test);

} // namespace args
} // namespace oct
