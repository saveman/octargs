#include "gtest/gtest.h"

#include <array>

#include "../include/octargs/internal/char_utils.hpp"

namespace oct
{
namespace args
{

TEST(char_utils_test, test_char_is_space)
{
    ASSERT_TRUE(internal::is_space(' '));
    ASSERT_TRUE(internal::is_space('\t'));
    ASSERT_TRUE(internal::is_space('\r'));
    ASSERT_TRUE(internal::is_space('\n'));
    ASSERT_TRUE(!internal::is_space('a'));
    ASSERT_TRUE(!internal::is_space('1'));
    ASSERT_TRUE(!internal::is_space('#'));
}

TEST(char_utils_test, test_wchar_is_space)
{
    ASSERT_TRUE(internal::is_space(L' '));
    ASSERT_TRUE(internal::is_space(L'\t'));
    ASSERT_TRUE(internal::is_space(L'\r'));
    ASSERT_TRUE(internal::is_space(L'\n'));
    ASSERT_TRUE(!internal::is_space(L'a'));
    ASSERT_TRUE(!internal::is_space(L'1'));
    ASSERT_TRUE(!internal::is_space(L'#'));
}

TEST(char_utils_test, test_char_to_lower)
{
    ASSERT_TRUE(internal::to_lower('c') == 'c');
    ASSERT_TRUE(internal::to_lower('C') == 'c');
    ASSERT_TRUE(internal::to_lower('1') == '1');
    ASSERT_TRUE(internal::to_lower('#') == '#');
}

TEST(char_utils_test, test_wchar_to_lower)
{
    ASSERT_TRUE(internal::to_lower(L'c') == L'c');
    ASSERT_TRUE(internal::to_lower(L'C') == L'c');
    ASSERT_TRUE(internal::to_lower(L'1') == L'1');
    ASSERT_TRUE(internal::to_lower(L'#') == L'#');
}

} // namespace args
} // namespace oct
