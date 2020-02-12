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
        argument arg(argument_kind::VALUED, names, true);
        CPPUNIT_ASSERT_EQUAL(argument_kind::VALUED, arg.get_kind());
        CPPUNIT_ASSERT_EQUAL(true, arg.is_required());
        CPPUNIT_ASSERT_EQUAL(names.size(), arg.get_names().size());
        for (std::size_t i = 0; i < names.size(); ++i)
        {
            CPPUNIT_ASSERT_EQUAL(names[i], arg.get_names()[i]);
        }
    }

    void test_multi_name()
    {
        argument::string_vector names = { "a", "bbb", "cc" };
        argument arg(argument_kind::VALUED, names, true);
        CPPUNIT_ASSERT_EQUAL(argument_kind::VALUED, arg.get_kind());
        CPPUNIT_ASSERT_EQUAL(true, arg.is_required());
        CPPUNIT_ASSERT_EQUAL(names.size(), arg.get_names().size());
        for (std::size_t i = 0; i < names.size(); ++i)
        {
            CPPUNIT_ASSERT_EQUAL(names[i], arg.get_names()[i]);
        }
    }

    void test_kind()
    {
        argument::string_vector names = { "a" };

        argument arg1(argument_kind::SWITCH, names, false);
        CPPUNIT_ASSERT_EQUAL(argument_kind::SWITCH, arg1.get_kind());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_required());

        argument arg2(argument_kind::VALUED, names, false);
        CPPUNIT_ASSERT_EQUAL(argument_kind::VALUED, arg2.get_kind());
        CPPUNIT_ASSERT_EQUAL(false, arg2.is_required());

        argument arg3(argument_kind::POSITIONAL, names, true);
        CPPUNIT_ASSERT_EQUAL(argument_kind::POSITIONAL, arg3.get_kind());
        CPPUNIT_ASSERT_EQUAL(true, arg3.is_required());
    }

    CPPUNIT_TEST_SUITE(argument_test);
    CPPUNIT_TEST(test_single_name);
    CPPUNIT_TEST(test_multi_name);
    CPPUNIT_TEST(test_kind);
    CPPUNIT_TEST_SUITE_END();
}; // namespace args

CPPUNIT_TEST_SUITE_REGISTRATION(argument_test);

} // namespace args
} // namespace oct
