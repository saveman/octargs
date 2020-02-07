#include <array>

#include "test_fixture.hpp"

#include "../include/octargs/basic_argument.hpp"
#include "../include/octargs/traits.hpp"

namespace oct
{
namespace args
{

class argument_test : public test_fixture
{
private:
    using argument = basic_argument<traits<char>>;

    void test_single_name()
    {
        argument::string_vector names = { "a" };
        argument arg(names, false);
        CPPUNIT_ASSERT_EQUAL(false, arg.has_value());
        CPPUNIT_ASSERT_EQUAL(names.size(), arg.get_names().size());
        for (std::size_t i = 0; i < names.size(); ++i)
        {
            CPPUNIT_ASSERT_EQUAL(names[i], arg.get_names()[i]);
        }
    }

    void test_multi_name()
    {
        argument::string_vector names = { "a", "bbb", "cc" };
        argument arg(names, false);
        CPPUNIT_ASSERT_EQUAL(false, arg.has_value());
        CPPUNIT_ASSERT_EQUAL(names.size(), arg.get_names().size());
        for (std::size_t i = 0; i < names.size(); ++i)
        {
            CPPUNIT_ASSERT_EQUAL(names[i], arg.get_names()[i]);
        }
    }

    void test_has_value()
    {
        argument::string_vector names = { "a" };

        argument arg1(names, false);
        CPPUNIT_ASSERT_EQUAL(false, arg1.has_value());

        argument arg2(names, true);
        CPPUNIT_ASSERT_EQUAL(true, arg2.has_value());
    }

    CPPUNIT_TEST_SUITE(argument_test);
    CPPUNIT_TEST(test_single_name);
    CPPUNIT_TEST(test_multi_name);
    CPPUNIT_TEST(test_has_value);
    CPPUNIT_TEST_SUITE_END();
}; // namespace args

CPPUNIT_TEST_SUITE_REGISTRATION(argument_test);

} // namespace args
} // namespace oct
