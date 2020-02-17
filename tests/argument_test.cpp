#include "../include/octargs/argument.hpp"

#include <array>

#include "test_fixture.hpp"

#include "../include/octargs/traits.hpp"

namespace oct
{
namespace args
{

class argument_test : public test_fixture
{
private:
    using string_vector_type = traits<char>::string_vector_type;
    using basic_argument_type = basic_argument<traits<char>, internal::null_values_storage>;
    using switch_argument_type = basic_switch_argument<traits<char>>;
    using valued_argument_type = basic_valued_argument<traits<char>>;
    using positional_argument_type = basic_positional_argument<traits<char>>;

    void test_single_name()
    {
        string_vector_type names = { "a" };
        switch_argument_type arg(names);
        CPPUNIT_ASSERT_EQUAL(false, arg.is_required());
        CPPUNIT_ASSERT_EQUAL(names.size(), arg.get_names().size());
        for (std::size_t i = 0; i < names.size(); ++i)
        {
            CPPUNIT_ASSERT_EQUAL(names[i], arg.get_names()[i]);
        }
    }

    void test_multi_name()
    {
        string_vector_type names = { "a", "bbb", "cc" };
        switch_argument_type arg(names);
        CPPUNIT_ASSERT_EQUAL(false, arg.is_required());
        CPPUNIT_ASSERT_EQUAL(names.size(), arg.get_names().size());
        for (std::size_t i = 0; i < names.size(); ++i)
        {
            CPPUNIT_ASSERT_EQUAL(names[i], arg.get_names()[i]);
        }
    }

    void test_switch_type()
    {
        string_vector_type names = { "a" };

        switch_argument_type arg1(names);
        CPPUNIT_ASSERT_EQUAL(true, arg1.is_assignable_by_name());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_accepting_immediate_value());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_accepting_separate_value());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_required());
    }

    void test_valued_type()
    {
        string_vector_type names = { "a" };

        valued_argument_type arg1(names);
        CPPUNIT_ASSERT_EQUAL(true, arg1.is_assignable_by_name());
        CPPUNIT_ASSERT_EQUAL(true, arg1.is_accepting_immediate_value());
        CPPUNIT_ASSERT_EQUAL(true, arg1.is_accepting_separate_value());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_required());
    }

    void test_positional_type()
    {
        string_vector_type names = { "a" };

        positional_argument_type arg1(names, false, false);
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_assignable_by_name());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_accepting_immediate_value());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_accepting_separate_value());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_required());
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), arg1.get_max_count());

        positional_argument_type arg2(names, true, true);
        CPPUNIT_ASSERT_EQUAL(false, arg2.is_assignable_by_name());
        CPPUNIT_ASSERT_EQUAL(false, arg2.is_accepting_immediate_value());
        CPPUNIT_ASSERT_EQUAL(false, arg2.is_accepting_separate_value());
        CPPUNIT_ASSERT_EQUAL(true, arg2.is_required());
        CPPUNIT_ASSERT_GREATER(static_cast<std::size_t>(1000), arg2.get_max_count());
    }

    CPPUNIT_TEST_SUITE(argument_test);
    CPPUNIT_TEST(test_single_name);
    CPPUNIT_TEST(test_multi_name);
    CPPUNIT_TEST(test_switch_type);
    CPPUNIT_TEST(test_valued_type);
    CPPUNIT_TEST(test_positional_type);
    CPPUNIT_TEST_SUITE_END();
}; // namespace args

CPPUNIT_TEST_SUITE_REGISTRATION(argument_test);

} // namespace args
} // namespace oct
