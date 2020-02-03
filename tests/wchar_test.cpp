#include "test_fixture.hpp"

#include "parser.hpp"

namespace oct
{
namespace args
{

    class wchar_test : public test_fixture
    {
    private:
        void test_basic()
        {
            int test_argc = 3;
            const wchar_t* test_argv[] = {
                L"appname",
                L"param1",
                L"param2",
                L"param3",
            };

            wparser parser;
            parser.set_positional_arguments_enabled(true);

            parser.parse(test_argc, test_argv);
            // TODO: add asserts
        }

        CPPUNIT_TEST_SUITE(wchar_test);
        CPPUNIT_TEST(test_basic);
        CPPUNIT_TEST_SUITE_END();
    };

    CPPUNIT_TEST_SUITE_REGISTRATION(wchar_test);

} // namespace args
} // namespace oct
