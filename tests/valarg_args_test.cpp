#include "test_fixture.hpp"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

class valarg_args_test : public test_fixture
{
private:
    void test_happy_hapth()
    {
        argument_table args("appname", { "-a", "valuea", "-c", "valuec", "-a", "valueb" });

        parser parser;
        parser.add_valarg({ "-a" }).set_unlimited_count();
        parser.add_valarg({ "-b" });
        parser.add_valarg({ "-c" });

        auto results = parser.parse(args);

        CPPUNIT_ASSERT(results.has_value("-a"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(2), results.values("-a").size());
        CPPUNIT_ASSERT_EQUAL(std::string("valuea"), results.values("-a")[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("valueb"), results.values("-a")[1]);

        CPPUNIT_ASSERT(!results.has_value("-b"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(0), results.values("-b").size());

        CPPUNIT_ASSERT(results.has_value("-c"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results.values("-c").size());
        CPPUNIT_ASSERT_EQUAL(std::string("valuec"), results.values("-c")[0]);
    }

    void test_no_names()
    {
        parser parser;
        CPPUNIT_ASSERT_THROW(parser.add_valarg({}), configuration_exception);
    }

    void test_multiple_names()
    {
        argument_table args("appname", { "-a", "val1", "-d", "val2", "--beta", "val3" });

        parser parser;
        parser.add_valarg({ "-a", "--alpha" });
        parser.add_valarg({ "-b", "--beta", "beatrice" });
        parser.add_valarg({ "-c", "--charlie" });
        parser.add_valarg({ "-d", "--delta" });

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
        CPPUNIT_ASSERT_THROW(parser.add_valarg({ "" }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_valarg({ "\t\n\r" }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_valarg({ "a " }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_valarg({ " b" }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_valarg({ " c " }), configuration_exception);
    }

    void test_duplicated_names()
    {
        parser parser;
        CPPUNIT_ASSERT_THROW(parser.add_valarg({ "-h", "-h" }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_valarg({ "-v", "--version", "-v" }), configuration_exception);
    }

    void test_conflicting_names()
    {
        parser parser;
        parser.add_valarg({ "-h" });
        CPPUNIT_ASSERT_THROW(parser.add_valarg({ "-h" }), configuration_exception);
    }

    void test_duplicated_value()
    {
        argument_table args("appname", { "-v", "0", "-v", "1" });

        parser parser;
        parser.add_valarg({ "-v" });

        CPPUNIT_ASSERT_THROW(parser.parse(args), parse_exception);
    }

    void test_multiple_values()
    {
        argument_table args1("appname", { "-v", "0" });
        argument_table args2("appname", { "-v", "0", "-v", "1", "-a", "0" });
        argument_table args4("appname", { "-v", "0", "-v", "2", "-v", "0", "-v", "1" });

        parser parser;
        parser.add_valarg({ "-v" }).set_max_count(4);
        parser.add_valarg({ "-a" }).set_max_count(2);

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
        argument_table args("appname", { "-v", "1", "-v", "2", "-v", "3" });

        parser parser;
        parser.add_valarg({ "-v" }).set_max_count(2);

        CPPUNIT_ASSERT_THROW(parser.parse(args), parse_exception);
    }

    void test_unlimited_values()
    {
        const std::size_t COUNT = 543;
        argument_table args("appname", std::vector<std::string>(COUNT, "--arg"));

        parser parser;
        parser.add_valarg({ "--arg" }).set_unlimited_count();

        auto results = parser.parse(args);
        CPPUNIT_ASSERT_EQUAL(COUNT, results.count("--arg"));
    }

    void test_missing_value()
    {
        argument_table args1("appname", { "-v" });
        argument_table args2("appname", { "-a", "1", "-v" });

        parser parser;
        parser.add_valarg({ "-a" });
        parser.add_valarg({ "-v" });

        CPPUNIT_ASSERT_THROW(parser.parse(args1), parse_exception);
        CPPUNIT_ASSERT_THROW(parser.parse(args2), parse_exception);
    }

    void test_value_is_arg_name()
    {
        argument_table args("appname", { "-v", "-a", "-a", "-v" });

        parser parser;
        parser.add_valarg({ "-a" });
        parser.add_valarg({ "-v" });

        auto results = parser.parse(args);

        CPPUNIT_ASSERT(results.has_value("-a"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results.values("-a").size());
        CPPUNIT_ASSERT_EQUAL(std::string("-v"), results.values("-a")[0]);

        CPPUNIT_ASSERT(results.has_value("-v"));
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results.values("-v").size());
        CPPUNIT_ASSERT_EQUAL(std::string("-a"), results.values("-v")[0]);
    }

    void test_switch_valarg_name_conflict()
    {
        parser parser;
        parser.add_switch({ "-s", "--switch" });
        parser.add_valarg({ "-v", "--value" });
        CPPUNIT_ASSERT_THROW(parser.add_valarg({ "-s" }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_valarg({ "--switch" }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_switch({ "-v" }), configuration_exception);
        CPPUNIT_ASSERT_THROW(parser.add_switch({ "--value" }), configuration_exception);
    }

    CPPUNIT_TEST_SUITE(valarg_args_test);
    CPPUNIT_TEST(test_happy_hapth);
    CPPUNIT_TEST(test_no_names);
    CPPUNIT_TEST(test_multiple_names);
    CPPUNIT_TEST(test_invalid_name);
    CPPUNIT_TEST(test_duplicated_names);
    CPPUNIT_TEST(test_conflicting_names);
    CPPUNIT_TEST(test_duplicated_value);
    CPPUNIT_TEST(test_multiple_values);
    CPPUNIT_TEST(test_more_values_than_count);
    CPPUNIT_TEST(test_missing_value);
    CPPUNIT_TEST(test_value_is_arg_name);
    CPPUNIT_TEST(test_switch_valarg_name_conflict);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(valarg_args_test);

} // namespace args
} // namespace oct
