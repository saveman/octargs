#include "test_fixture.hpp"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

class switch_args_test : public test_fixture
{
private:
    void test_happy_hapth()
    {
        argument_table args("appname", { "-a", "-c" });

        parser parser;
        parser.add_switch({ "-a" });
        parser.add_switch({ "-b" });
        parser.add_switch({ "-c" });

        auto results = parser.parse(args);
        CPPUNIT_ASSERT(results.has_value("-a"));
        CPPUNIT_ASSERT(!results.has_value("-b"));
        CPPUNIT_ASSERT(results.has_value("-c"));
    }

    void test_no_names()
    {
        parser parser;
        CPPUNIT_ASSERT_THROW(parser.add_switch({}), configuration_exception);
    }

    void test_multiple_names()
    {
        argument_table args("appname", { "-a", "-d", "--beta" });

        parser parser;
        parser.add_switch({ "-a", "--alpha" });
        parser.add_switch({ "-b", "--beta", "beatrice" });
        parser.add_switch({ "-c", "--charlie" });
        parser.add_switch({ "-d", "--delta" });

        auto results = parser.parse(args);
        CPPUNIT_ASSERT(results.has_value("-a"));
        CPPUNIT_ASSERT(results.has_value("--alpha"));
        CPPUNIT_ASSERT(results.has_value("-b"));
        CPPUNIT_ASSERT(results.has_value("--beta"));
        CPPUNIT_ASSERT(results.has_value("beatrice"));
        CPPUNIT_ASSERT(!results.has_value("-c"));
        CPPUNIT_ASSERT(!results.has_value("--charlie"));
        CPPUNIT_ASSERT(results.has_value("-d"));
        CPPUNIT_ASSERT(results.has_value("--delta"));
    }

    void test_invalid_name()
    {
        parser parser;
        CPPUNIT_ASSERT_THROW(parser.add_switch({ "" }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_switch({ "\t\n\r" }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_switch({ "a " }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_switch({ " b" }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_switch({ " c " }), configuration_exception);
    }

    void test_duplicated_names()
    {
        parser parser;
        CPPUNIT_ASSERT_THROW(parser.add_switch({ "-h", "-h" }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_switch({ "-v", "--version", "-v" }), configuration_exception);
    }

    void test_conflicting_names()
    {
        parser parser;
        parser.add_switch({ "-h" });
        CPPUNIT_ASSERT_THROW(parser.add_switch({ "-h" }), configuration_exception);
    }

    void test_duplicated_value()
    {
        argument_table args("appname", { "-v", "-v" });

        parser parser;
        parser.add_switch({ "-v" });

        CPPUNIT_ASSERT_THROW(parser.parse(args), parse_exception);
    }

    CPPUNIT_TEST_SUITE(switch_args_test);
    CPPUNIT_TEST(test_happy_hapth);
    CPPUNIT_TEST(test_no_names);
    CPPUNIT_TEST(test_multiple_names);
    CPPUNIT_TEST(test_invalid_name);
    CPPUNIT_TEST(test_duplicated_names);
    CPPUNIT_TEST(test_conflicting_names);
    CPPUNIT_TEST(test_duplicated_value);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(switch_args_test);

} // namespace args
} // namespace oct
