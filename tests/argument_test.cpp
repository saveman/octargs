#include "../include/octargs/argument.hpp"
#include "../include/octargs/exclusive_argument.hpp"
#include "../include/octargs/positional_argument.hpp"
#include "../include/octargs/switch_argument.hpp"
#include "../include/octargs/valued_argument.hpp"

#include <array>

#include "test_fixture.hpp"

namespace oct
{
namespace args
{

class argument_test : public test_fixture
{
private:
    using string_type = std::string;
    using string_vector_type = std::vector<string_type>;

    using basic_argument_type = basic_argument<char, internal::null_values_storage>;
    using switch_argument_type = basic_switch_argument<char>;
    using valued_argument_type = basic_valued_argument<char>;
    using positional_argument_type = basic_positional_argument<char>;
    using exclusive_argument_type = basic_exclusive_argument<char>;

    void test_single_name()
    {
        string_vector_type names = { "a" };
        switch_argument_type arg(names);
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), arg.get_min_count());
        CPPUNIT_ASSERT_EQUAL(std::size_t(1), arg.get_max_count());
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
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), arg.get_min_count());
        CPPUNIT_ASSERT_EQUAL(std::size_t(1), arg.get_max_count());
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
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_exclusive());
        CPPUNIT_ASSERT_EQUAL(true, arg1.is_assignable_by_name());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_accepting_immediate_value());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_accepting_separate_value());
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), arg1.get_min_count());
        CPPUNIT_ASSERT_EQUAL(std::size_t(1), arg1.get_max_count());
    }

    void test_exclusive_type()
    {
        string_vector_type names = { "a" };

        exclusive_argument_type arg1(names);
        CPPUNIT_ASSERT_EQUAL(true, arg1.is_exclusive());
        CPPUNIT_ASSERT_EQUAL(true, arg1.is_assignable_by_name());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_accepting_immediate_value());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_accepting_separate_value());
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), arg1.get_min_count());
        CPPUNIT_ASSERT_EQUAL(std::size_t(1), arg1.get_max_count());
    }

    void test_valued_type()
    {
        string_vector_type names = { "a" };

        valued_argument_type arg1(names);
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_exclusive());
        CPPUNIT_ASSERT_EQUAL(true, arg1.is_assignable_by_name());
        CPPUNIT_ASSERT_EQUAL(true, arg1.is_accepting_immediate_value());
        CPPUNIT_ASSERT_EQUAL(true, arg1.is_accepting_separate_value());
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), arg1.get_min_count());
        CPPUNIT_ASSERT_EQUAL(std::size_t(1), arg1.get_max_count());
    }

    void test_positional_type()
    {
        string_vector_type names = { "a" };

        positional_argument_type arg1(names);
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_exclusive());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_assignable_by_name());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_accepting_immediate_value());
        CPPUNIT_ASSERT_EQUAL(false, arg1.is_accepting_separate_value());
        CPPUNIT_ASSERT_EQUAL(std::size_t(0), arg1.get_min_count());
        CPPUNIT_ASSERT_EQUAL(std::size_t(1), arg1.get_max_count());
        CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), arg1.get_max_count());

        positional_argument_type arg2(names);
        arg2.set_min_count(3).set_max_count(10);
        CPPUNIT_ASSERT_EQUAL(false, arg2.is_assignable_by_name());
        CPPUNIT_ASSERT_EQUAL(false, arg2.is_accepting_immediate_value());
        CPPUNIT_ASSERT_EQUAL(false, arg2.is_accepting_separate_value());
        CPPUNIT_ASSERT_EQUAL(std::size_t(3), arg2.get_min_count());
        CPPUNIT_ASSERT_EQUAL(std::size_t(10), arg2.get_max_count());
        arg2.set_max_count_unlimited();
        CPPUNIT_ASSERT_GREATER(static_cast<std::size_t>(1000), arg2.get_max_count());
    }

    CPPUNIT_TEST_SUITE(argument_test);
    CPPUNIT_TEST(test_single_name);
    CPPUNIT_TEST(test_multi_name);
    CPPUNIT_TEST(test_switch_type);
    CPPUNIT_TEST(test_exclusive_type);
    CPPUNIT_TEST(test_valued_type);
    CPPUNIT_TEST(test_positional_type);
    CPPUNIT_TEST_SUITE_END();
}; // namespace args

CPPUNIT_TEST_SUITE_REGISTRATION(argument_test);

} // namespace args
} // namespace oct
