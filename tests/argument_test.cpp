#include "gtest/gtest.h"

#include <array>

#include "../include/octargs/exclusive_argument.hpp"
#include "../include/octargs/positional_argument.hpp"
#include "../include/octargs/switch_argument.hpp"
#include "../include/octargs/valued_argument.hpp"

namespace oct
{
namespace args
{

namespace
{

using string_type = std::string;
using string_vector_type = std::vector<string_type>;

using dictionary_type = default_dictionary<char>;

using exclusive_argument_type = internal::basic_exclusive_argument_impl<char, void>;
using positional_argument_type = internal::basic_positional_argument_impl<char, void>;
using switch_argument_type = internal::basic_switch_argument_impl<char, void>;
using valued_argument_type = internal::basic_valued_argument_impl<char, void>;

} // namespace

TEST(argument_test, test_single_name)
{
    string_vector_type names = { "a" };
    switch_argument_type arg(names);
    ASSERT_EQ(std::size_t(0), arg.get_min_count());
    ASSERT_EQ(std::size_t(1), arg.get_max_count());
    ASSERT_EQ(names.size(), arg.get_names().size());
    for (std::size_t i = 0; i < names.size(); ++i)
    {
        ASSERT_EQ(names[i], arg.get_names()[i]);
    }
}

TEST(argument_test, test_multi_name)
{
    string_vector_type names = { "a", "bbb", "cc" };
    switch_argument_type arg(names);
    ASSERT_EQ(std::size_t(0), arg.get_min_count());
    ASSERT_EQ(std::size_t(1), arg.get_max_count());
    ASSERT_EQ(names.size(), arg.get_names().size());
    for (std::size_t i = 0; i < names.size(); ++i)
    {
        ASSERT_EQ(names[i], arg.get_names()[i]);
    }
}

TEST(argument_test, test_switch_type)
{
    string_vector_type names = { "a" };

    switch_argument_type arg1(names);
    ASSERT_EQ(false, arg1.is_exclusive());
    ASSERT_EQ(true, arg1.is_assignable_by_name());
    ASSERT_EQ(false, arg1.is_accepting_immediate_value());
    ASSERT_EQ(false, arg1.is_accepting_separate_value());
    ASSERT_EQ(std::size_t(0), arg1.get_min_count());
    ASSERT_EQ(std::size_t(1), arg1.get_max_count());
}

TEST(argument_test, test_exclusive_type)
{
    string_vector_type names = { "a" };

    exclusive_argument_type arg1(names);
    ASSERT_EQ(true, arg1.is_exclusive());
    ASSERT_EQ(true, arg1.is_assignable_by_name());
    ASSERT_EQ(false, arg1.is_accepting_immediate_value());
    ASSERT_EQ(false, arg1.is_accepting_separate_value());
    ASSERT_EQ(std::size_t(0), arg1.get_min_count());
    ASSERT_EQ(std::size_t(1), arg1.get_max_count());
}

TEST(argument_test, test_valued_type)
{
    string_vector_type names = { "a" };

    valued_argument_type arg1(names);
    ASSERT_EQ(false, arg1.is_exclusive());
    ASSERT_EQ(true, arg1.is_assignable_by_name());
    ASSERT_EQ(true, arg1.is_accepting_immediate_value());
    ASSERT_EQ(true, arg1.is_accepting_separate_value());
    ASSERT_EQ(std::size_t(0), arg1.get_min_count());
    ASSERT_EQ(std::size_t(1), arg1.get_max_count());
}

TEST(argument_test, test_positional_type)
{
    string_vector_type names = { "a" };

    positional_argument_type arg1(names);
    ASSERT_EQ(false, arg1.is_exclusive());
    ASSERT_EQ(false, arg1.is_assignable_by_name());
    ASSERT_EQ(false, arg1.is_accepting_immediate_value());
    ASSERT_EQ(false, arg1.is_accepting_separate_value());
    ASSERT_EQ(std::size_t(0), arg1.get_min_count());
    ASSERT_EQ(std::size_t(1), arg1.get_max_count());
    ASSERT_EQ(static_cast<std::size_t>(1), arg1.get_max_count());

    positional_argument_type arg2(names);
    arg2.set_min_count(3);
    arg2.set_max_count(10);
    ASSERT_EQ(false, arg2.is_assignable_by_name());
    ASSERT_EQ(false, arg2.is_accepting_immediate_value());
    ASSERT_EQ(false, arg2.is_accepting_separate_value());
    ASSERT_EQ(std::size_t(3), arg2.get_min_count());
    ASSERT_EQ(std::size_t(10), arg2.get_max_count());
    arg2.set_max_count_unlimited();
    ASSERT_TRUE(arg2.is_max_count_unlimited());
}

} // namespace args
} // namespace oct
