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

} // namespace args
} // namespace oct
