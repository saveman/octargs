#include <array>

#include "test_fixture.hpp"

#include "arguments.hpp"

namespace oct
{
namespace args
{

    class arguments_test : public test_fixture
    {
    private:
        void test_argc_argv_version()
        {
            char app[] { "app" };
            char arg1[] { "arg1" };
            char arg2[] { "arg2" };
            char arg3[] { "arg3" };
            char* argv[] = { app, arg1, arg2, arg3, nullptr };
            size_t argc = 4;
            arguments args(argc, argv);

            CPPUNIT_ASSERT_EQUAL(std::string("app"), args.get_app_name());
            CPPUNIT_ASSERT_EQUAL(argc - 1, args.get_argument_count());
            CPPUNIT_ASSERT_EQUAL(std::string("arg1"), args.get_argument(0));
            CPPUNIT_ASSERT_EQUAL(std::string("arg2"), args.get_argument(1));
            CPPUNIT_ASSERT_EQUAL(std::string("arg3"), args.get_argument(2));
        }

        void test_string_version()
        {
            std::vector<std::string> argv = { "arg1", "arg2", "arg3" };
            arguments args("app", argv);

            CPPUNIT_ASSERT_EQUAL(std::string("app"), args.get_app_name());
            CPPUNIT_ASSERT_EQUAL(argv.size(), args.get_argument_count());
            CPPUNIT_ASSERT_EQUAL(std::string("arg1"), args.get_argument(0));
            CPPUNIT_ASSERT_EQUAL(std::string("arg2"), args.get_argument(1));
            CPPUNIT_ASSERT_EQUAL(std::string("arg3"), args.get_argument(2));
        }

        CPPUNIT_TEST_SUITE(arguments_test);
        CPPUNIT_TEST(test_argc_argv_version);
        CPPUNIT_TEST(test_string_version);
        CPPUNIT_TEST_SUITE_END();
    };

    CPPUNIT_TEST_SUITE_REGISTRATION(arguments_test);

} // namespace args
} // namespace oct
