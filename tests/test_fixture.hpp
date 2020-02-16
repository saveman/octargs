#ifndef OCTARGS_TESTS_TEST_FIXTURE_HPP_
#define OCTARGS_TESTS_TEST_FIXTURE_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace oct
{
namespace args
{

class test_fixture : public CppUnit::TestFixture
{
public:
    void setUp() override
    {
        set_up();
    }

    void tearDown() override
    {
        tear_down();
    }

    virtual void set_up()
    {
        // noop
    }

    virtual void tear_down()
    {
        // noop
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_TESTS_TEST_FIXTURE_HPP_
