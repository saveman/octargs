#include "test_fixture.hpp"

#include "exception.hpp"

namespace oct
{
namespace args
{

    class exception_test : public test_fixture
    {
    private:
        void construction_test()
        {
            exception exception_no_args;
            CPPUNIT_ASSERT(exception_no_args.what() == std::string());

            const std::string message("Test Message");
            exception exception_with_message(message);
            CPPUNIT_ASSERT(exception_with_message.what() == message);
        }

        void throw_test()
        {
        }

        CPPUNIT_TEST_SUITE(exception_test);
        CPPUNIT_TEST(construction_test);
        CPPUNIT_TEST(throw_test);
        CPPUNIT_TEST_SUITE_END();
    };

} // namespace args
} // namespace oct
