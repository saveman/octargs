#include <array>
#include <limits>
#include <type_traits>

#include "test_fixture.hpp"

#include "../include/octargs/converter.hpp"
#include "../include/octargs/traits.hpp"

namespace oct
{
namespace args
{

class converter_test : public test_fixture
{
private:
    void test_string_converter()
    {
        using traits_type = traits<char>;
        using string_type = traits_type::string_type;
        basic_converter<traits_type, string_type> converter;

        CPPUNIT_ASSERT_EQUAL(string_type(""), converter(""));
        CPPUNIT_ASSERT_EQUAL(string_type("x"), converter("x"));
        CPPUNIT_ASSERT_EQUAL(string_type("alamakota"), converter("alamakota"));
        CPPUNIT_ASSERT_EQUAL(string_type(" abc"), converter(" abc"));
        CPPUNIT_ASSERT_EQUAL(string_type("abc "), converter("abc "));
        CPPUNIT_ASSERT_EQUAL(string_type("  abc\t\n"), converter("  abc\t\n"));
    }

    void test_wstring_converter()
    {
        using traits_type = traits<wchar_t>;
        using string_type = traits_type::string_type;
        basic_converter<traits_type, string_type> converter;

        CPPUNIT_ASSERT(string_type(L"") == converter(L""));
        CPPUNIT_ASSERT(string_type(L"x") == converter(L"x"));
        CPPUNIT_ASSERT(string_type(L"alamakota") == converter(L"alamakota"));
        CPPUNIT_ASSERT(string_type(L" abc") == converter(L" abc"));
        CPPUNIT_ASSERT(string_type(L"abc ") == converter(L"abc "));
        CPPUNIT_ASSERT(string_type(L"  abc\t\n") == converter(L"  abc\t\n"));
    }

    template <typename CHAR>
    void test_bool_converter_helper()
    {
        using char_type = CHAR;
        using char_traits_type = traits<char_type>;
        basic_converter<char_traits_type, bool> char_converter;

        CPPUNIT_ASSERT_EQUAL(true, char_converter(char_traits_type::get_switch_enabled_literal()));

        auto true_values = char_traits_type::get_true_literals();
        for (auto value : true_values)
        {
            CPPUNIT_ASSERT_EQUAL(true, char_converter(value));
        }

        auto false_values = char_traits_type::get_false_literals();
        for (auto value : false_values)
        {
            CPPUNIT_ASSERT_EQUAL(false, char_converter(value));
        }
    }

    void test_bool_converter()
    {
        test_bool_converter_helper<char>();

        basic_converter<traits<char>, bool> char_converter;
        CPPUNIT_ASSERT_THROW(char_converter(""), parse_exception);
        CPPUNIT_ASSERT_THROW(char_converter("a"), parse_exception);
        CPPUNIT_ASSERT_THROW(char_converter(" 1"), parse_exception);

        test_bool_converter_helper<wchar_t>();

        basic_converter<traits<wchar_t>, bool> wchart_converter;
        CPPUNIT_ASSERT_THROW(wchart_converter(L""), parse_exception);
        CPPUNIT_ASSERT_THROW(wchart_converter(L"a"), parse_exception);
        CPPUNIT_ASSERT_THROW(wchart_converter(L" 1"), parse_exception);
    }

    template <typename VALUE>
    void test_integer_converter()
    {
        using value_type = VALUE;
        using traits_type = traits<char>;
        using limits_type = std::numeric_limits<value_type>;

        basic_converter<traits_type, value_type> converter;

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

        CPPUNIT_ASSERT_THROW(converter(""), parse_exception);
        CPPUNIT_ASSERT_THROW(converter("a"), parse_exception);
        CPPUNIT_ASSERT_THROW(converter("15a"), parse_exception);
        CPPUNIT_ASSERT_THROW(converter("15 15"), parse_exception);
        CPPUNIT_ASSERT_THROW(converter("078"), parse_exception);
        CPPUNIT_ASSERT_THROW(converter("0xfg"), parse_exception);
        CPPUNIT_ASSERT_THROW(converter("2+2"), parse_exception);
        CPPUNIT_ASSERT_THROW(converter("111111111111111111111111"), parse_exception);
        CPPUNIT_ASSERT_THROW(converter("-111111111111111111111111"), parse_exception);
    }

    template <typename VALUE>
    void test_float_converter()
    {
        using value_type = VALUE;
        using traits_type = traits<char>;

        basic_converter<traits_type, value_type> converter;

        value_type epsylon = 0.001;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(static_cast<value_type>(0), converter("0"), epsylon);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(static_cast<value_type>(163), converter("163"), epsylon);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(static_cast<value_type>(0.5), converter("0.5"), epsylon);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(static_cast<value_type>(-21.3), converter("-21.3"), epsylon);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(static_cast<value_type>(-0.004), converter("-0.004"), epsylon);

        CPPUNIT_ASSERT_THROW(converter(""), parse_exception);
        CPPUNIT_ASSERT_THROW(converter("a"), parse_exception);
        CPPUNIT_ASSERT_THROW(converter("15a"), parse_exception);
        CPPUNIT_ASSERT_THROW(converter("15 15"), parse_exception);
        CPPUNIT_ASSERT_THROW(converter("0xfg"), parse_exception);
        CPPUNIT_ASSERT_THROW(converter("2+2"), parse_exception);
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
