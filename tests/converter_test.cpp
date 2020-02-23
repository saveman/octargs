#include <array>
#include <limits>
#include <type_traits>

#include "test_fixture.hpp"

#include "../include/octargs/converter.hpp"

namespace oct
{
namespace args
{

class converter_test : public test_fixture
{
private:
    void test_string_converter()
    {
        using char_type = char;
        using string_type = std::basic_string<char_type>;

        basic_converter<char_type, string_type> converter;

        CPPUNIT_ASSERT_EQUAL(string_type(""), converter(""));
        CPPUNIT_ASSERT_EQUAL(string_type("x"), converter("x"));
        CPPUNIT_ASSERT_EQUAL(string_type("alamakota"), converter("alamakota"));
        CPPUNIT_ASSERT_EQUAL(string_type(" abc"), converter(" abc"));
        CPPUNIT_ASSERT_EQUAL(string_type("abc "), converter("abc "));
        CPPUNIT_ASSERT_EQUAL(string_type("  abc\t\n"), converter("  abc\t\n"));
    }

    void test_wstring_converter()
    {
        using char_type = wchar_t;
        using string_type = std::basic_string<char_type>;

        basic_converter<char_type, string_type> converter;

        CPPUNIT_ASSERT(string_type(L"") == converter(L""));
        CPPUNIT_ASSERT(string_type(L"x") == converter(L"x"));
        CPPUNIT_ASSERT(string_type(L"alamakota") == converter(L"alamakota"));
        CPPUNIT_ASSERT(string_type(L" abc") == converter(L" abc"));
        CPPUNIT_ASSERT(string_type(L"abc ") == converter(L"abc "));
        CPPUNIT_ASSERT(string_type(L"  abc\t\n") == converter(L"  abc\t\n"));
    }

    template <typename char_T>
    void test_bool_converter_helper()
    {
        using char_type = char_T;
        using dictionary_type = parser_dictionary<char_type>;

        basic_converter<char_type, bool> char_converter;

        CPPUNIT_ASSERT_EQUAL(true, char_converter(dictionary_type::get_switch_enabled_literal()));

        auto true_values = dictionary_type::get_true_literals();
        for (auto value : true_values)
        {
            CPPUNIT_ASSERT_EQUAL(true, char_converter(value));
        }

        auto false_values = dictionary_type::get_false_literals();
        for (auto value : false_values)
        {
            CPPUNIT_ASSERT_EQUAL(false, char_converter(value));
        }
    }

    void test_bool_converter()
    {
        test_bool_converter_helper<char>();

        basic_converter<char, bool> char_converter;
        CPPUNIT_ASSERT_THROW(char_converter(""), conversion_error);
        CPPUNIT_ASSERT_THROW(char_converter("a"), conversion_error);
        CPPUNIT_ASSERT_THROW(char_converter(" 1"), conversion_error);

        test_bool_converter_helper<wchar_t>();

        basic_converter<wchar_t, bool> wchart_converter;
        CPPUNIT_ASSERT_THROW(wchart_converter(L""), conversion_error);
        CPPUNIT_ASSERT_THROW(wchart_converter(L"a"), conversion_error);
        CPPUNIT_ASSERT_THROW(wchart_converter(L" 1"), conversion_error);

        try
        {
            wchart_converter(L"a");
            CPPUNIT_ASSERT(false);
        }
        catch (const conversion_error_ex<wchar_t>& exc)
        {
            CPPUNIT_ASSERT(std::wstring(L"a") == exc.value());
        }
    }

    template <typename value_T>
    void test_integer_converter()
    {
        using char_type = char;
        using value_type = value_T;
        using limits_type = std::numeric_limits<value_type>;

        basic_converter<char_type, value_type> converter;

        CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(0), converter("0"));
        CPPUNIT_ASSERT_EQUAL(limits_type::min(), converter(std::to_string(limits_type::min())));
        CPPUNIT_ASSERT_EQUAL(limits_type::max(), converter(std::to_string(limits_type::max())));

        CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(10), converter("10"));
        CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(21), converter("0x15"));
        CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(15), converter("017"));

        if (std::is_signed<value_type>())
        {
            CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(-500), converter("-500"));
            CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(-4095), converter("-0xFFF"));
        }

        CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(13), converter(" 13"));
        CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(13), converter("13 "));
        CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(13), converter(" 13 "));

        CPPUNIT_ASSERT_THROW(converter(""), conversion_error);
        CPPUNIT_ASSERT_THROW(converter("a"), conversion_error);
        CPPUNIT_ASSERT_THROW(converter("15a"), conversion_error);
        CPPUNIT_ASSERT_THROW(converter("15 15"), conversion_error);
        CPPUNIT_ASSERT_THROW(converter("078"), conversion_error);
        CPPUNIT_ASSERT_THROW(converter("0xfg"), conversion_error);
        CPPUNIT_ASSERT_THROW(converter("2+2"), conversion_error);
        CPPUNIT_ASSERT_THROW(converter("111111111111111111111111"), conversion_error);
        CPPUNIT_ASSERT_THROW(converter("-111111111111111111111111"), conversion_error);

        try
        {
            converter("0xfg");
            CPPUNIT_ASSERT(false);
        }
        catch (const conversion_error_ex<char_type>& exc)
        {
            CPPUNIT_ASSERT_EQUAL(std::string("0xfg"), exc.value());
        }
    }

    template <typename value_T>
    void test_float_converter()
    {
        using char_type = char;
        using value_type = value_T;

        basic_converter<char_type, value_type> converter;

        value_type epsylon = 0.001;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(static_cast<value_type>(0), converter("0"), epsylon);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(static_cast<value_type>(163), converter("163"), epsylon);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(static_cast<value_type>(0.5), converter("0.5"), epsylon);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(static_cast<value_type>(-21.3), converter("-21.3"), epsylon);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(static_cast<value_type>(-0.004), converter("-0.004"), epsylon);

        CPPUNIT_ASSERT_THROW(converter(""), conversion_error);
        CPPUNIT_ASSERT_THROW(converter("a"), conversion_error);
        CPPUNIT_ASSERT_THROW(converter("15a"), conversion_error);
        CPPUNIT_ASSERT_THROW(converter("15 15"), conversion_error);
        CPPUNIT_ASSERT_THROW(converter("0xfg"), conversion_error);
        CPPUNIT_ASSERT_THROW(converter("2+2"), conversion_error);

        try
        {
            converter("15 15");
            CPPUNIT_ASSERT(false);
        }
        catch (const conversion_error_ex<char_type>& exc)
        {
            CPPUNIT_ASSERT_EQUAL(std::string("15 15"), exc.value());
        }
    }

    CPPUNIT_TEST_SUITE(converter_test);
    CPPUNIT_TEST(test_string_converter);
    CPPUNIT_TEST(test_wstring_converter);
    CPPUNIT_TEST(test_bool_converter);
    CPPUNIT_TEST(test_integer_converter<int>);
    CPPUNIT_TEST(test_integer_converter<unsigned int>);
    CPPUNIT_TEST(test_integer_converter<long>);
    CPPUNIT_TEST(test_integer_converter<unsigned long>);
    CPPUNIT_TEST(test_integer_converter<long long>);
    CPPUNIT_TEST(test_integer_converter<unsigned long long>);
    CPPUNIT_TEST(test_integer_converter<short>);
    CPPUNIT_TEST(test_integer_converter<unsigned short>);
    CPPUNIT_TEST(test_integer_converter<std::int32_t>);
    CPPUNIT_TEST(test_integer_converter<std::uint32_t>);
    CPPUNIT_TEST(test_float_converter<float>);
    CPPUNIT_TEST(test_float_converter<double>);
    CPPUNIT_TEST(test_float_converter<long double>);
    CPPUNIT_TEST_SUITE_END();
}; // namespace args

CPPUNIT_TEST_SUITE_REGISTRATION(converter_test);

} // namespace args
} // namespace oct
