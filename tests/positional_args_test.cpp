#include "test_fixture.hpp"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

class positional_args_test : public test_fixture
{
private:
    void test_not_allowed_default()
    {
        argument_table args("appname", { "argument1" });

        parser parser;

        CPPUNIT_ASSERT_THROW(parser.parse(args), exception);
    }

    void test_not_allowed_no_arguments()
    {
        argument_table args("appname", {});

        parser parser;
        parser.set_positional_arguments_enabled(false);

        auto results = parser.parse(args);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(0), results.get_positional_arguments().size());
    }

    void test_not_allowed_with_arguments()
    {
        argument_table args("appname", { "argument1" });

        parser parser;
        parser.set_positional_arguments_enabled(false);

        CPPUNIT_ASSERT_THROW(parser.parse(args), exception);
    }

    void test_not_allowed_default_with_switch()
    {
        argument_table args("appname", { "-v", "argument1" });

        parser parser;
        parser.add_switch({ "-v" });

        CPPUNIT_ASSERT_THROW(parser.parse(args), exception);
    }

    void test_not_allowed_no_arguments_with_switch()
    {
        argument_table args("appname", { "-v" });

        parser parser;
        parser.add_switch({ "-v" });
        parser.set_positional_arguments_enabled(false);

        auto results = parser.parse(args);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(0), results.get_positional_arguments().size());
    }

    void test_not_allowed_with_arguments_with_switch()
    {
        argument_table args("appname", { "-v", "argument1" });

        parser parser;
        parser.add_switch({ "-v" });
        parser.set_positional_arguments_enabled(false);

        CPPUNIT_ASSERT_THROW(parser.parse(args), exception);
    }

    void test_allowed_no_arguments()
    {
        argument_table args("appname", {});

        parser parser;
        parser.set_positional_arguments_enabled(true);

        auto results = parser.parse(args);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(0), results.get_positional_arguments().size());
    }

    void test_allowed_single_argument()
    {
        argument_table args("appname", { "argument1" });

        parser parser;
        parser.set_positional_arguments_enabled(true);

        auto results = parser.parse(args);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results.get_positional_arguments().size());
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results.get_positional_arguments()[0]);
    }

    void test_allowed_multi_argument()
    {
        argument_table args("appname", { "argument1", "argument2", "argument3" });

        parser parser;
        parser.set_positional_arguments_enabled(true);

        auto results = parser.parse(args);

        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(3), results.get_positional_arguments().size());
        CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results.get_positional_arguments()[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument2"), results.get_positional_arguments()[1]);
        CPPUNIT_ASSERT_EQUAL(std::string("argument3"), results.get_positional_arguments()[2]);
    }

    // TODO: tests with a named arguments (allowed, scenarios without and with positional arguments)

    CPPUNIT_TEST_SUITE(positional_args_test);
    CPPUNIT_TEST(test_not_allowed_default);
    CPPUNIT_TEST(test_not_allowed_no_arguments);
    CPPUNIT_TEST(test_not_allowed_with_arguments);
    CPPUNIT_TEST(test_not_allowed_default_with_switch);
    CPPUNIT_TEST(test_not_allowed_no_arguments_with_switch);
    CPPUNIT_TEST(test_not_allowed_with_arguments_with_switch);
    CPPUNIT_TEST(test_allowed_no_arguments);
    CPPUNIT_TEST(test_allowed_single_argument);
    CPPUNIT_TEST(test_allowed_multi_argument);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(positional_args_test);

} // namespace args
} // namespace oct
