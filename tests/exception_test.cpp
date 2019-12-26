#include "test_fixture.hpp"

#include "exception.hpp"

namespace oct
{
namespace args
{

    class exception_test : public test_fixture
    {
    private:
        void test_constructors()
        {
            exception exception_no_args;
            CPPUNIT_ASSERT(exception_no_args.what() == std::string());

            const std::string message("Test Message");
            exception exception_with_message(message);
            CPPUNIT_ASSERT(exception_with_message.what() == message);
        }

        void test_throwing()
        {
            const std::string message("Test Message");
            try
            {
                throw exception(message);
                CPPUNIT_ASSERT(false);
            }
            catch (const exception& e)
            {
                CPPUNIT_ASSERT(e.what() == message);
            }
            catch (...)
            {
                CPPUNIT_ASSERT(false);
            }
        }

        CPPUNIT_TEST_SUITE(exception_test);
        CPPUNIT_TEST(test_constructors);
        CPPUNIT_TEST(test_throwing);
        CPPUNIT_TEST_SUITE_END();
    };

    CPPUNIT_TEST_SUITE_REGISTRATION(exception_test);

} // namespace args
} // namespace oct
