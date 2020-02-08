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
        argument arg(names);
        CPPUNIT_ASSERT_EQUAL(false, arg.is_value_required());
        CPPUNIT_ASSERT_EQUAL(names.size(), arg.get_names().size());
        for (std::size_t i = 0; i < names.size(); ++i)
        {
            CPPUNIT_ASSERT_EQUAL(names[i], arg.get_names()[i]);
        }
    }

    void test_multi_name()
    {
        argument::string_vector names = { "a", "bbb", "cc" };
        argument arg(names);
        CPPUNIT_ASSERT_EQUAL(false, arg.is_value_required());
        CPPUNIT_ASSERT_EQUAL(names.size(), arg.get_names().size());
        for (std::size_t i = 0; i < names.size(); ++i)
        {
            CPPUNIT_ASSERT_EQUAL(names[i], arg.get_names()[i]);
        }
    }

    void test_has_value()
    {
        argument::string_vector names = { "a" };

        argument arg1(names);
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_value_required());

        // TODO: enabled when ready
        // argument arg2(names);
        // CPPUNIT_ASSERT_EQUAL(true, arg2.is_value_required());
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
