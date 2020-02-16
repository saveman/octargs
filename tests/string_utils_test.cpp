#include <array>

#include "test_fixture.hpp"

#include "../include/octargs/internal/string_utils.hpp"
#include "../include/octargs/traits.hpp"

namespace oct
{
namespace args
{

class string_utils_test : public test_fixture
{
private:
    template <typename VALUE>
    void test_stox()
    {
        using value_type = VALUE;
        using string_type = std::string;
        using limits_type = std::numeric_limits<value_type>;

        long long min_value = limits_type::min();
        long long max_value = limits_type::max();

        CPPUNIT_ASSERT_EQUAL(value_type(0), (internal::stox<string_type, value_type>(std::string("0"), nullptr, 0)));
        CPPUNIT_ASSERT_EQUAL(
            value_type(min_value), (internal::stox<string_type, value_type>(std::to_string(min_value), nullptr, 0)));
        CPPUNIT_ASSERT_EQUAL(
            value_type(max_value), (internal::stox<string_type, value_type>(std::to_string(max_value), nullptr, 0)));

        CPPUNIT_ASSERT_THROW(
            (internal::stox<string_type, value_type>(std::to_string(min_value - 1), nullptr, 0)), std::out_of_range);
        CPPUNIT_ASSERT_THROW(
            (internal::stox<string_type, value_type>(std::to_string(max_value + 1), nullptr, 0)), std::out_of_range);

        std::size_t end_pos;
        CPPUNIT_ASSERT_EQUAL(
            value_type(10), (internal::stox<string_type, value_type>(std::string("10a"), &end_pos, 0)));
        CPPUNIT_ASSERT_EQUAL(std::size_t(2), end_pos);
    }

    void test_trim_char()
    {
        using string_type = std::string;

        const string_type TRIM_LEAD_INPUT(" \tAAA \n");
        const string_type TRIM_LEAD_OUTPUT("AAA \n");
        const string_type TRIM_TRAIL_INPUT(" \t BBB  \n");
        const string_type TRIM_TRAIL_OUTPUT(" \t BBB");
        const string_type TRIM_INPUT("\r\nCCCC\t\t");
        const string_type TRIM_OUTPUT("CCCC");

        string_type tmp;

        tmp = TRIM_LEAD_INPUT;
        CPPUNIT_ASSERT_EQUAL(TRIM_LEAD_OUTPUT, internal::trim_lead_copy(tmp));
        CPPUNIT_ASSERT_EQUAL(TRIM_LEAD_INPUT, tmp);
        internal::trim_lead(tmp);
        CPPUNIT_ASSERT_EQUAL(TRIM_LEAD_OUTPUT, tmp);

        tmp = TRIM_TRAIL_INPUT;
        CPPUNIT_ASSERT_EQUAL(TRIM_TRAIL_OUTPUT, internal::trim_trail_copy(tmp));
        CPPUNIT_ASSERT_EQUAL(TRIM_TRAIL_INPUT, tmp);
        internal::trim_trail(tmp);
        CPPUNIT_ASSERT_EQUAL(TRIM_TRAIL_OUTPUT, tmp);

        tmp = TRIM_INPUT;
        CPPUNIT_ASSERT_EQUAL(TRIM_OUTPUT, internal::trim_copy(tmp));
        CPPUNIT_ASSERT_EQUAL(TRIM_INPUT, tmp);
        internal::trim(tmp);
        CPPUNIT_ASSERT_EQUAL(TRIM_OUTPUT, tmp);
    }

    void test_trim_wchar()
    {
        using string_type = std::wstring;

        const string_type TRIM_LEAD_INPUT(L" \tAAA \n");
        const string_type TRIM_LEAD_OUTPUT(L"AAA \n");
        const string_type TRIM_TRAIL_INPUT(L" \t BBB  \n");
        const string_type TRIM_TRAIL_OUTPUT(L" \t BBB");
        const string_type TRIM_INPUT(L"\r\nCCCC\t\t");
        const string_type TRIM_OUTPUT(L"CCCC");

        string_type tmp;

        tmp = TRIM_LEAD_INPUT;
        CPPUNIT_ASSERT(TRIM_LEAD_OUTPUT == internal::trim_lead_copy(tmp));
        CPPUNIT_ASSERT(TRIM_LEAD_INPUT == tmp);
        internal::trim_lead(tmp);
        CPPUNIT_ASSERT(TRIM_LEAD_OUTPUT == tmp);

        tmp = TRIM_TRAIL_INPUT;
        CPPUNIT_ASSERT(TRIM_TRAIL_OUTPUT == internal::trim_trail_copy(tmp));
        CPPUNIT_ASSERT(TRIM_TRAIL_INPUT == tmp);
        internal::trim_trail(tmp);
        CPPUNIT_ASSERT(TRIM_TRAIL_OUTPUT == tmp);

        tmp = TRIM_INPUT;
        CPPUNIT_ASSERT(TRIM_OUTPUT == internal::trim_copy(tmp));
        CPPUNIT_ASSERT(TRIM_INPUT == tmp);
        internal::trim(tmp);
        CPPUNIT_ASSERT(TRIM_OUTPUT == tmp);
    }

    CPPUNIT_TEST_SUITE(string_utils_test);
    CPPUNIT_TEST(test_stox<short>);
    CPPUNIT_TEST(test_stox<unsigned short>);
    CPPUNIT_TEST(test_stox<unsigned int>);
    CPPUNIT_TEST(test_trim_wchar);
    CPPUNIT_TEST(test_trim_char);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(string_utils_test);

} // namespace args
} // namespace oct
