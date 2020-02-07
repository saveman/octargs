#include "test_fixture.hpp"

#include "../include/octargs/basic_parser.hpp"
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
            L"param1",
            L"param2",
            L"param3",
        };
        int test_argc = sizeof(test_argv) / sizeof(test_argv[0]);

        wparser parser;
        parser.set_positional_arguments_enabled(true);

        auto results = parser.parse(test_argc, test_argv);
        CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), results.get_positional_arguments().size());
        CPPUNIT_ASSERT(results.get_positional_arguments()[0] == test_argv[1]);
        CPPUNIT_ASSERT(results.get_positional_arguments()[1] == test_argv[2]);
        CPPUNIT_ASSERT(results.get_positional_arguments()[2] == test_argv[3]);
    }

    CPPUNIT_TEST_SUITE(wchar_test);
    CPPUNIT_TEST(test_basic);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(wchar_test);

} // namespace args
} // namespace oct
