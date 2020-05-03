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
    ASSERT_EQ(value_type(min_value), (internal::stox<string_type, value_type>(std::to_string(min_value), nullptr, 0)));
    ASSERT_EQ(value_type(max_value), (internal::stox<string_type, value_type>(std::to_string(max_value), nullptr, 0)));

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

TEST(string_utils_test, test_string_less_char)
{
    internal::string_less<char> comparator_case(true);

    ASSERT_TRUE(comparator_case("aaa", "aab"));
    ASSERT_FALSE(comparator_case("aab", "aaa"));
    ASSERT_FALSE(comparator_case("aaa", "aaa"));
    ASSERT_TRUE(comparator_case("aaa", "aaaa"));
    ASSERT_FALSE(comparator_case("aaaa", "aaa"));

    internal::string_less<char> comparator_nocase(false);

    ASSERT_TRUE(comparator_nocase("aaa", "AaB"));
    ASSERT_FALSE(comparator_nocase("AAB", "aaa"));
    ASSERT_FALSE(comparator_nocase("aaa", "AaA"));
    ASSERT_TRUE(comparator_case("AAA", "aaaa"));
    ASSERT_FALSE(comparator_case("aaAA", "Aaa"));
}

TEST(string_utils_test, test_string_less_wchart)
{
    internal::string_less<wchar_t> comparator_case(true);

    ASSERT_TRUE(comparator_case(L"aaa", L"aab"));
    ASSERT_FALSE(comparator_case(L"aab", L"aaa"));
    ASSERT_FALSE(comparator_case(L"aaa", L"aaa"));
    ASSERT_TRUE(comparator_case(L"aaa", L"aaaa"));
    ASSERT_FALSE(comparator_case(L"aaaa", L"aaa"));

    internal::string_less<wchar_t> comparator_nocase(false);

    ASSERT_TRUE(comparator_nocase(L"aaa", L"AaB"));
    ASSERT_FALSE(comparator_nocase(L"AAB", L"aaa"));
    ASSERT_FALSE(comparator_nocase(L"aaa", L"AaA"));
    ASSERT_TRUE(comparator_case(L"AAA", L"aaaa"));
    ASSERT_FALSE(comparator_case(L"aaAA", L"Aaa"));
}

TEST(string_utils_test, test_string_equal_char)
{
    internal::string_equal<char> comparator_case(true);

    ASSERT_TRUE(comparator_case("aaa", "aaa"));
    ASSERT_FALSE(comparator_case("aaa", "AAA"));
    ASSERT_FALSE(comparator_case("aab", "aaa"));
    ASSERT_FALSE(comparator_case("aaa", "aa"));
    ASSERT_FALSE(comparator_case("aaa", "aaaa"));
    ASSERT_FALSE(comparator_case("aaaa", "aaa"));

    internal::string_equal<char> comparator_nocase(false);

    ASSERT_TRUE(comparator_nocase("aaa", "aaa"));
    ASSERT_TRUE(comparator_nocase("aaA", "AAa"));
    ASSERT_FALSE(comparator_nocase("AAB", "aaa"));
    ASSERT_FALSE(comparator_nocase("aaa", "Aa"));
    ASSERT_FALSE(comparator_case("AAA", "aaaa"));
    ASSERT_FALSE(comparator_case("aaAA", "Aaa"));
}

TEST(string_utils_test, test_string_equal_wchart)
{
    internal::string_equal<wchar_t> comparator_case(true);

    ASSERT_TRUE(comparator_case(L"aaa", L"aaa"));
    ASSERT_FALSE(comparator_case(L"aaa", L"AAA"));
    ASSERT_FALSE(comparator_case(L"aab", L"aaa"));
    ASSERT_FALSE(comparator_case(L"aaa", L"aa"));
    ASSERT_FALSE(comparator_case(L"aaa", L"aaaa"));
    ASSERT_FALSE(comparator_case(L"aaaa", L"aaa"));

    internal::string_equal<wchar_t> comparator_nocase(false);

    ASSERT_TRUE(comparator_nocase(L"aaa", L"aaa"));
    ASSERT_TRUE(comparator_nocase(L"aaA", L"AAa"));
    ASSERT_FALSE(comparator_nocase(L"AAB", L"aaa"));
    ASSERT_FALSE(comparator_nocase(L"aaa", L"Aa"));
    ASSERT_FALSE(comparator_case(L"AAA", L"aaaa"));
    ASSERT_FALSE(comparator_case(L"aaAA", L"Aaa"));
}

} // namespace args
} // namespace oct
