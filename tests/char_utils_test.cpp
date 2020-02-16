#include <array>

#include "test_fixture.hpp"

#include "../include/octargs/internal/char_utils.hpp"

namespace oct
{
namespace args
{

class char_utils_test : public test_fixture
{
private:
    void test_char_is_space()
    {
        CPPUNIT_ASSERT(internal::is_space(' '));
        CPPUNIT_ASSERT(internal::is_space('\t'));
        CPPUNIT_ASSERT(internal::is_space('\r'));
        CPPUNIT_ASSERT(internal::is_space('\n'));
        CPPUNIT_ASSERT(!internal::is_space('a'));
        CPPUNIT_ASSERT(!internal::is_space('1'));
        CPPUNIT_ASSERT(!internal::is_space('#'));
    }

    void test_wchar_is_space()
    {
        CPPUNIT_ASSERT(internal::is_space(L' '));
        CPPUNIT_ASSERT(internal::is_space(L'\t'));
        CPPUNIT_ASSERT(internal::is_space(L'\r'));
        CPPUNIT_ASSERT(internal::is_space(L'\n'));
        CPPUNIT_ASSERT(!internal::is_space(L'a'));
        CPPUNIT_ASSERT(!internal::is_space(L'1'));
        CPPUNIT_ASSERT(!internal::is_space(L'#'));
    }

    CPPUNIT_TEST_SUITE(char_utils_test);
    CPPUNIT_TEST(test_char_is_space);
    CPPUNIT_TEST(test_wchar_is_space);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(char_utils_test);

} // namespace args
} // namespace oct
