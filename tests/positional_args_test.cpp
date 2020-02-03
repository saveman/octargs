#include "test_fixture.hpp"

#include "argument_table.hpp"
#include "parser.hpp"

namespace oct
{
namespace args
{

    class positional_args_test : public test_fixture
    {
    private:
        void test_not_allowed_default()
        {
            parser parser_defaults;

            CPPUNIT_ASSERT_THROW(parser_defaults.parse(argument_table("appname", { "argument1" })), exception);
        }

        void test_not_allowed_no_arguments()
        {
            parser parser_no_args;
            parser_no_args.set_positional_arguments_enabled(false);

            auto& results = parser_no_args.parse(argument_table("appname", {}));

            CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(0), results.get_positional_arguments().size());
        }

        void test_not_allowed_with_arguments()
        {
            parser parser_args;
            parser_args.set_positional_arguments_enabled(false);

            CPPUNIT_ASSERT_THROW(parser_args.parse(argument_table("appname", { "argument1" })), exception);
        }

        // TODO: tests with a named arguments (not allowed, scenarios without and with positional arguments)

        void test_allowed_no_arguments()
        {
            parser parser_no_args;
            parser_no_args.set_positional_arguments_enabled(true);

            auto& results = parser_no_args.parse(argument_table("appname", {}));

            CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(0), results.get_positional_arguments().size());
        }

        void test_allowed_single_argument()
        {
            parser parser_single_arg;
            parser_single_arg.set_positional_arguments_enabled(true);

            auto& results = parser_single_arg.parse(argument_table("appname", { "argument1" }));

            CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), results.get_positional_arguments().size());
            CPPUNIT_ASSERT_EQUAL(std::string("argument1"), results.get_positional_arguments()[0]);
        }

        void test_allowed_multi_argument()
        {
            parser parser_multi_arg;
            parser_multi_arg.set_positional_arguments_enabled(true);

            auto& results
                = parser_multi_arg.parse(argument_table("appname", { "argument1", "argument2", "argument3" }));

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
        CPPUNIT_TEST(test_allowed_no_arguments);
        CPPUNIT_TEST(test_allowed_single_argument);
        CPPUNIT_TEST(test_allowed_multi_argument);
        CPPUNIT_TEST_SUITE_END();
    };

    CPPUNIT_TEST_SUITE_REGISTRATION(positional_args_test);

} // namespace args
} // namespace oct
