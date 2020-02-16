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
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results.values("-a").size());
        CPPUNIT_ASSERT_EQUAL(traits<char>::get_switch_enabled_literal(), results.values("-a")[0]);
        CPPUNIT_ASSERT(!results.has_value("-b"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(0), results.values("-b").size());
        CPPUNIT_ASSERT(results.has_value("-c"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results.values("-c").size());
        CPPUNIT_ASSERT_EQUAL(traits<char>::get_switch_enabled_literal(), results.values("-c")[0]);
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
        CPPUNIT_ASSERT_THROW(parser.add_switch({ "=c" }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_switch({ "c=" }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_switch({ "c==c" }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_switch({ "=" }), configuration_exception);
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

    void test_multiple_values()
    {
        argument_table args1("appname", { "-v" });
        argument_table args2("appname", { "-v", "-v", "-a" });
        argument_table args4("appname", { "-v", "-v", "-v", "-v" });

        parser parser;
        parser.add_switch({ "-v" }).set_max_count(4);
        parser.add_switch({ "-a" }).set_max_count(2);

        auto results1 = parser.parse(args1);
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results1.count("-v"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(0), results1.count("-a"));
        auto results2 = parser.parse(args2);
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(2), results2.count("-v"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results2.count("-a"));
        auto results4 = parser.parse(args4);
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(4), results4.count("-v"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(0), results4.count("-a"));
    }

    void test_more_values_than_count()
    {
        argument_table args("appname", { "-v", "-v", "-v" });

        parser parser;
        parser.add_switch({ "-v" }).set_max_count(2);

        CPPUNIT_ASSERT_THROW(parser.parse(args), parse_exception);
    }

    void test_unlimited_values()
    {
        const std::size_t COUNT = 543;
        argument_table args("appname", std::vector<std::string>(COUNT, "--arg"));

        parser parser;
        parser.add_switch({ "--arg" }).set_unlimited_count();

        auto results = parser.parse(args);
        CPPUNIT_ASSERT_EQUAL(COUNT, results.count("--arg"));
    }

    CPPUNIT_TEST_SUITE(switch_args_test);
    CPPUNIT_TEST(test_happy_hapth);
    CPPUNIT_TEST(test_no_names);
    CPPUNIT_TEST(test_multiple_names);
    CPPUNIT_TEST(test_invalid_name);
    CPPUNIT_TEST(test_duplicated_names);
    CPPUNIT_TEST(test_conflicting_names);
    CPPUNIT_TEST(test_duplicated_value);
    CPPUNIT_TEST(test_multiple_values);
    CPPUNIT_TEST(test_more_values_than_count);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(switch_args_test);

} // namespace args
} // namespace oct
