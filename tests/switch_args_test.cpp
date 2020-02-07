#include "test_fixture.hpp"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

class switch_args_test : public test_fixture
{
private:
    void test_no_names()
    {
        parser parser;
        CPPUNIT_ASSERT_THROW(parser.add_switch({}), configuration_exception);
    }

    void test_too_many_names()
    {
        parser parser;
        CPPUNIT_ASSERT_THROW(parser.add_switch({ "-h", "--help" }), configuration_exception);
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
        // TODO: This actually only passes because it fails with 'too many names'
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
    CPPUNIT_TEST(test_no_names);
    CPPUNIT_TEST(test_too_many_names);
    CPPUNIT_TEST(test_invalid_name);
    CPPUNIT_TEST(test_duplicated_names);
    CPPUNIT_TEST(test_conflicting_names);
    CPPUNIT_TEST(test_duplicated_value);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(switch_args_test);

} // namespace args
} // namespace oct
