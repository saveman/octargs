#include "gtest/gtest.h"

#include <array>

#include "../include/octargs/argument_table.hpp"

namespace oct
{
namespace args
{

namespace
{

using argument_table = basic_argument_table<char>;
using argument_table_iterator = basic_argument_table_iterator<char>;

} // namespace

TEST(argument_table_test, test_argc_argv_version)
{
    char app[] { "app" };
    char arg1[] { "arg1" };
    char arg2[] { "arg2" };
    char arg3[] { "arg3" };
    char* argv[] = { app, arg1, arg2, arg3, nullptr };
    int argc = 4;
    argument_table args(argc, argv);

    ASSERT_EQ(std::string("app"), args.get_app_name());
    ASSERT_EQ(static_cast<std::size_t>(argc - 1), args.get_argument_count());
    ASSERT_EQ(std::string("arg1"), args.get_argument(0));
    ASSERT_EQ(std::string("arg2"), args.get_argument(1));
    ASSERT_EQ(std::string("arg3"), args.get_argument(2));
}

TEST(argument_table_test, test_string_version)
{
    std::vector<std::string> argv = { "arg1", "arg2", "arg3" };
    argument_table args("app", argv);

    ASSERT_EQ(std::string("app"), args.get_app_name());
    ASSERT_EQ(argv.size(), args.get_argument_count());
    ASSERT_EQ(std::string("arg1"), args.get_argument(0));
    ASSERT_EQ(std::string("arg2"), args.get_argument(1));
    ASSERT_EQ(std::string("arg3"), args.get_argument(2));
}

TEST(argument_table_test, test_iterator)
{
    argument_table args("app", { "arg1", "arg2" });

    argument_table_iterator iter(args);
    ASSERT_TRUE(iter.has_more());
    ASSERT_EQ(std::string("arg1"), iter.peek_next());
    ASSERT_EQ(std::string("arg1"), iter.take_next());
    ASSERT_TRUE(iter.has_more());
    ASSERT_EQ(std::string("arg2"), iter.peek_next());
    ASSERT_EQ(std::string("arg2"), iter.peek_next());
    ASSERT_EQ(std::string("arg2"), iter.take_next());
    ASSERT_TRUE(!iter.has_more());
    ASSERT_THROW(iter.peek_next(), std::out_of_range);
    ASSERT_THROW(iter.take_next(), std::out_of_range);
    ASSERT_TRUE(!iter.has_more());
}

} // namespace args
} // namespace oct
