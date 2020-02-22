#include "test_fixture.hpp"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

class exclusive_args_test : public test_fixture
{
private:
    void test_not_given_no_args()
    {
        argument_table args("appname", {});

        parser parser;
        parser.add_exclusive({ "--help" });
        parser.add_exclusive({ "--version" });
        parser.add_switch({ "--verbose" });
        parser.add_positional("files").set_max_count_unlimited();

        auto results = parser.parse(args);
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), results.count("--help"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), results.count("--version"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), results.count("--verbose"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), results.count("files"));
    }

    void test_switch_given()
    {
        argument_table args("appname", { "--verbose" });

        parser parser;
        parser.add_exclusive({ "--help" });
        parser.add_exclusive({ "--version" });
        parser.add_switch({ "--verbose" });
        parser.add_positional("files").set_max_count_unlimited();

        auto results = parser.parse(args);
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), results.count("--help"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), results.count("--version"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(1), results.count("--verbose"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), results.count("files"));
    }

    void test_one_given()
    {
        argument_table args("appname", { "--version" });

        parser parser;
        parser.add_exclusive({ "--help" });
        parser.add_exclusive({ "--version" });
        parser.add_switch({ "--verbose" });
        parser.add_positional("files").set_max_count_unlimited();

        auto results = parser.parse(args);
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), results.count("--help"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(1), results.count("--version"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), results.count("--verbose"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), results.count("files"));
    }

    void test_two_given()
    {
        argument_table args("appname", { "--version", "--help" });

        parser parser;
        parser.add_exclusive({ "--help" });
        parser.add_exclusive({ "--version" });
        parser.add_switch({ "--verbose" });
        parser.add_positional("files").set_max_count_unlimited();

        auto results = parser.parse(args);
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), results.count("--help"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), results.count("--version"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), results.count("--verbose"));
        CPPUNIT_ASSERT_EQUAL(std::size_t(2), results.count("files"));
    }

    CPPUNIT_TEST_SUITE(exclusive_args_test);
    CPPUNIT_TEST(test_not_given_no_args);
    CPPUNIT_TEST(test_switch_given);
    CPPUNIT_TEST(test_one_given);
    CPPUNIT_TEST(test_two_given);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(exclusive_args_test);

} // namespace args
} // namespace oct
