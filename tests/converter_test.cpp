#include "gtest/gtest.h"

#include <array>
#include <limits>
#include <type_traits>

#include "../include/octargs/converter.hpp"

namespace oct
{
namespace args
{

TEST(converter_test, test_string_converter)
{
    using char_type = char;
    using string_type = std::basic_string<char_type>;

    basic_converter<char_type, string_type> converter;

    ASSERT_EQ(string_type(""), converter(""));
    ASSERT_EQ(string_type("x"), converter("x"));
    ASSERT_EQ(string_type("alamakota"), converter("alamakota"));
    ASSERT_EQ(string_type(" abc"), converter(" abc"));
    ASSERT_EQ(string_type("abc "), converter("abc "));
    ASSERT_EQ(string_type("  abc\t\n"), converter("  abc\t\n"));
}

TEST(converter_test, test_wstring_converter)
{
    using char_type = wchar_t;
    using string_type = std::basic_string<char_type>;

    basic_converter<char_type, string_type> converter;

    ASSERT_TRUE(string_type(L"") == converter(L""));
    ASSERT_TRUE(string_type(L"x") == converter(L"x"));
    ASSERT_TRUE(string_type(L"alamakota") == converter(L"alamakota"));
    ASSERT_TRUE(string_type(L" abc") == converter(L" abc"));
    ASSERT_TRUE(string_type(L"abc ") == converter(L"abc "));
    ASSERT_TRUE(string_type(L"  abc\t\n") == converter(L"  abc\t\n"));
}

template <typename char_T>
void test_bool_converter_helper()
{
    using char_type = char_T;
    using dictionary_type = default_parser_dictionary<char_type>;

    dictionary_type dictionary;

    basic_converter<char_type, bool> char_converter;

    ASSERT_EQ(true, char_converter(dictionary, dictionary.get_switch_enabled_literal()));

    auto true_values = dictionary.get_true_literals();
    for (auto value : true_values)
    {
        ASSERT_EQ(true, char_converter(dictionary, value));
    }

    auto false_values = dictionary.get_false_literals();
    for (auto value : false_values)
    {
        ASSERT_EQ(false, char_converter(dictionary, value));
    }
}

TEST(converter_test, test_bool_converter)
{
    test_bool_converter_helper<char>();

    basic_converter<char, bool> char_converter;
    default_parser_dictionary<char> char_dictionary;

    ASSERT_THROW(char_converter(char_dictionary, ""), conversion_error);
    ASSERT_THROW(char_converter(char_dictionary, "a"), conversion_error);
    ASSERT_THROW(char_converter(char_dictionary, " 1"), conversion_error);

    test_bool_converter_helper<wchar_t>();
    default_parser_dictionary<wchar_t> wchart_dictionary;

    basic_converter<wchar_t, bool> wchart_converter;
    ASSERT_THROW(wchart_converter(wchart_dictionary, L""), conversion_error);
    ASSERT_THROW(wchart_converter(wchart_dictionary, L"a"), conversion_error);
    ASSERT_THROW(wchart_converter(wchart_dictionary, L" 1"), conversion_error);

    try
    {
        wchart_converter(wchart_dictionary, L"a");
        ASSERT_TRUE(false);
    }
    catch (const conversion_error_ex<wchar_t>& exc)
    {
        ASSERT_TRUE(std::wstring(L"a") == exc.get_value());
    }
}

template <typename value_T>
void test_integer_converter()
{
    using char_type = char;
    using value_type = value_T;
    using limits_type = std::numeric_limits<value_type>;

    basic_converter<char_type, value_type> converter;

    ASSERT_EQ(static_cast<value_type>(0), converter("0"));
    ASSERT_EQ(limits_type::min(), converter(std::to_string(limits_type::min())));
    ASSERT_EQ(limits_type::max(), converter(std::to_string(limits_type::max())));

    ASSERT_EQ(static_cast<value_type>(10), converter("10"));
    ASSERT_EQ(static_cast<value_type>(21), converter("0x15"));
    ASSERT_EQ(static_cast<value_type>(15), converter("017"));

    if (std::is_signed<value_type>())
    {
        ASSERT_EQ(static_cast<value_type>(-500), converter("-500"));
        ASSERT_EQ(static_cast<value_type>(-4095), converter("-0xFFF"));
    }

    ASSERT_EQ(static_cast<value_type>(13), converter(" 13"));
    ASSERT_EQ(static_cast<value_type>(13), converter("13 "));
    ASSERT_EQ(static_cast<value_type>(13), converter(" 13 "));

    ASSERT_THROW(converter(""), conversion_error);
    ASSERT_THROW(converter("a"), conversion_error);
    ASSERT_THROW(converter("15a"), conversion_error);
    ASSERT_THROW(converter("15 15"), conversion_error);
    ASSERT_THROW(converter("078"), conversion_error);
    ASSERT_THROW(converter("0xfg"), conversion_error);
    ASSERT_THROW(converter("2+2"), conversion_error);
    ASSERT_THROW(converter("111111111111111111111111"), conversion_error);
    ASSERT_THROW(converter("-111111111111111111111111"), conversion_error);

    try
    {
        converter("0xfg");
        ASSERT_TRUE(false);
    }
    catch (const conversion_error_ex<char_type>& exc)
    {
        ASSERT_EQ(std::string("0xfg"), exc.get_value());
    }
}

template <typename value_T>
void test_float_converter()
{
    using char_type = char;
    using value_type = value_T;

    basic_converter<char_type, value_type> converter;

    ASSERT_DOUBLE_EQ(static_cast<value_type>(0), converter("0"));
    ASSERT_DOUBLE_EQ(static_cast<value_type>(163), converter("163"));
    ASSERT_DOUBLE_EQ(static_cast<value_type>(0.5), converter("0.5"));
    ASSERT_DOUBLE_EQ(static_cast<value_type>(-21.3), converter("-21.3"));
    ASSERT_DOUBLE_EQ(static_cast<value_type>(-0.004), converter("-0.004"));

    ASSERT_THROW(converter(""), conversion_error);
    ASSERT_THROW(converter("a"), conversion_error);
    ASSERT_THROW(converter("15a"), conversion_error);
    ASSERT_THROW(converter("15 15"), conversion_error);
    ASSERT_THROW(converter("0xfg"), conversion_error);
    ASSERT_THROW(converter("2+2"), conversion_error);

    try
    {
        converter("15 15");
        ASSERT_TRUE(false);
    }
    catch (const conversion_error_ex<char_type>& exc)
    {
        ASSERT_EQ(std::string("15 15"), exc.get_value());
    }
}

TEST(converter_test, test_integer_converter_int)
{
    test_integer_converter<int>();
}

TEST(converter_test, test_integer_converter_uint)
{
    test_integer_converter<unsigned int>();
}

TEST(converter_test, test_integer_converter_long)
{
    test_integer_converter<long>();
}

TEST(converter_test, test_integer_converter_ulong)
{
    test_integer_converter<unsigned long>();
}

TEST(converter_test, test_integer_converter_longlong)
{
    test_integer_converter<long long>();
}

TEST(converter_test, test_integer_converter_ulonglong)
{
    test_integer_converter<unsigned long long>();
}

TEST(converter_test, test_integer_converter_short)
{
    test_integer_converter<short>();
}

TEST(converter_test, test_integer_converter_ushort)
{
    test_integer_converter<unsigned short>();
}

TEST(converter_test, test_integer_converter_int32)
{
    test_integer_converter<std::int32_t>();
}

TEST(converter_test, test_integer_converter_uint32)
{
    test_integer_converter<std::uint32_t>();
}

TEST(converter_test, test_float_converter_float)
{
    test_float_converter<float>();
}

TEST(converter_test, test_float_converter_double)
{
    test_float_converter<double>();
}

TEST(converter_test, test_float_converter_longdouble)
{
    test_float_converter<long double>();
}

} // namespace args
} // namespace oct
