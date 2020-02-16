#include "../include/octargs/parser.hpp"
#include "test_fixture.hpp"

#include "../include/octargs/traits.hpp"

namespace oct
{
namespace args
{

class wchar_test : public test_fixture
{
private:
    using wparser = basic_parser<traits<wchar_t>>;

    void test_basic()
    {
        const wchar_t* test_argv[] = {
            L"appname",
            L"-v",
            L"param1",
            L"param2",
            L"param3",
        };
        int test_argc = sizeof(test_argv) / sizeof(test_argv[0]);

        wparser parser;
        parser.add_switch({ L"-v", L"--version" });
        parser.add_positional(L"values", false, true);

        auto results = parser.parse(test_argc, test_argv);
        CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), results.count(L"values"));
        CPPUNIT_ASSERT(results.values(L"values")[0] == test_argv[2]);
        CPPUNIT_ASSERT(results.values(L"values")[1] == test_argv[3]);
        CPPUNIT_ASSERT(results.values(L"values")[2] == test_argv[4]);
        CPPUNIT_ASSERT(results.has_value(L"--version"));
    }

    CPPUNIT_TEST_SUITE(wchar_test);
    CPPUNIT_TEST(test_basic);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(wchar_test);

} // namespace args
} // namespace oct
