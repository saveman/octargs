#include "gtest/gtest.h"

#include <array>

#include "../include/octargs/internal/string_utils.hpp"

namespace oct
{
namespace args
{

template <typename VALUE>
void test_stox()
{
    using value_type = VALUE;
    using string_type = std::string;
    using limits_type = std::numeric_limits<value_type>;

    long long min_value = limits_type::min();
    long long max_value = limits_type::max();

    ASSERT_EQ(value_type(0), (internal::stox<string_type, value_type>(std::string("0"), nullptr, 0)));
    ASSERT_EQ(
        value_type(min_value), (internal::stox<string_type, value_type>(std::to_string(min_value), nullptr, 0)));
    ASSERT_EQ(
        value_type(max_value), (internal::stox<string_type, value_type>(std::to_string(max_value), nullptr, 0)));

    ASSERT_THROW(
        (internal::stox<string_type, value_type>(std::to_string(min_value - 1), nullptr, 0)), std::out_of_range);
    ASSERT_THROW(
        (internal::stox<string_type, value_type>(std::to_string(max_value + 1), nullptr, 0)), std::out_of_range);

    std::size_t end_pos;
    ASSERT_EQ(value_type(10), (internal::stox<string_type, value_type>(std::string("10a"), &end_pos, 0)));
    ASSERT_EQ(std::size_t(2), end_pos);
}

TEST(string_utils_test, test_trim_char)
{
    using string_type = std::string;

    const string_type TRIM_LEAD_INPUT(" \tAAA \n");
    const string_type TRIM_LEAD_OUTPUT("AAA \n");
    const string_type TRIM_TRAIL_INPUT(" \t BBB  \n");
    const string_type TRIM_TRAIL_OUTPUT(" \t BBB");
    const string_type TRIM_INPUT("\r\nCCCC\t\t");
    const string_type TRIM_OUTPUT("CCCC");

    string_type tmp;
    string_type tmp_copy;

    tmp = TRIM_LEAD_INPUT;
    tmp_copy = TRIM_LEAD_INPUT;
    ASSERT_EQ(TRIM_LEAD_OUTPUT, internal::trim_lead_copy(tmp));
    ASSERT_EQ(tmp_copy, tmp);
    internal::trim_lead(tmp);
    ASSERT_EQ(TRIM_LEAD_OUTPUT, tmp);

    tmp = TRIM_TRAIL_INPUT;
    ASSERT_EQ(TRIM_TRAIL_OUTPUT, internal::trim_trail_copy(tmp));
    ASSERT_EQ(TRIM_TRAIL_INPUT, tmp);
    internal::trim_trail(tmp);
    ASSERT_EQ(TRIM_TRAIL_OUTPUT, tmp);

    tmp = TRIM_INPUT;
    ASSERT_EQ(TRIM_OUTPUT, internal::trim_copy(tmp));
    ASSERT_EQ(TRIM_INPUT, tmp);
    internal::trim(tmp);
    ASSERT_EQ(TRIM_OUTPUT, tmp);
}

TEST(string_utils_test, test_trim_wchar)
{
    using string_type = std::wstring;

    const string_type TRIM_LEAD_INPUT(L" \tAAA \n");
    const string_type TRIM_LEAD_OUTPUT(L"AAA \n");
    const string_type TRIM_TRAIL_INPUT(L" \t BBB  \n");
    const string_type TRIM_TRAIL_OUTPUT(L" \t BBB");
    const string_type TRIM_INPUT(L"\r\nCCCC\t\t");
    const string_type TRIM_OUTPUT(L"CCCC");

    string_type tmp;
    string_type tmp_copy;

    tmp = TRIM_LEAD_INPUT;
    tmp_copy = TRIM_LEAD_INPUT;
    ASSERT_TRUE(TRIM_LEAD_OUTPUT == internal::trim_lead_copy(tmp));
    ASSERT_TRUE(tmp_copy == tmp);
    internal::trim_lead(tmp);
    ASSERT_TRUE(TRIM_LEAD_OUTPUT == tmp);

    tmp = TRIM_TRAIL_INPUT;
    ASSERT_TRUE(TRIM_TRAIL_OUTPUT == internal::trim_trail_copy(tmp));
    ASSERT_TRUE(TRIM_TRAIL_INPUT == tmp);
    internal::trim_trail(tmp);
    ASSERT_TRUE(TRIM_TRAIL_OUTPUT == tmp);

    tmp = TRIM_INPUT;
    ASSERT_TRUE(TRIM_OUTPUT == internal::trim_copy(tmp));
    ASSERT_TRUE(TRIM_INPUT == tmp);
    internal::trim(tmp);
    ASSERT_TRUE(TRIM_OUTPUT == tmp);
}

TEST(string_utils_test, test_stox_short)
{
    test_stox<short>();
}

TEST(string_utils_test, test_stox_ushort)
{
    test_stox<unsigned short>();
}

TEST(string_utils_test, test_stox_uint)
{
    test_stox<unsigned int>();
}

} // namespace args
} // namespace oct
