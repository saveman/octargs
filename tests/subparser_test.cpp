#include "gtest/gtest.h"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

TEST(subparser_test, test_basic)
{
    parser parser;
    parser.add_switch({ "-v", "--verbose" });
    auto subparsers = parser.add_subparsers("command");

    auto add_parser = subparsers.add_parser("add");
    add_parser.add_switch({ "-s", "--show-steps" });
    add_parser.add_positional("values").set_max_count_unlimited();

    auto mul_parser = subparsers.add_parser("mul");
    mul_parser.add_switch({ "-s", "--show-steps" });
    mul_parser.add_positional("values").set_max_count_unlimited();

    argument_table args1("appname", { "-v", "add", "2", "4", "18" });

    auto results1 = parser.parse(args1);
    ASSERT_EQ(std::size_t(1), results1.get_count("--verbose"));
    ASSERT_EQ(std::size_t(1), results1.get_count("command"));
    ASSERT_EQ(std::string("add"), results1.get_values("command")[0]);
    ASSERT_EQ(std::size_t(0), results1.get_count("add -s"));
    ASSERT_EQ(std::size_t(3), results1.get_count("add values"));
    ASSERT_EQ(std::string("2"), results1.get_values("add values")[0]);
    ASSERT_EQ(std::string("4"), results1.get_values("add values")[1]);
    ASSERT_EQ(std::string("18"), results1.get_values("add values")[2]);
}

namespace
{

enum class command
{
    INVALID,
    ADD,
    MUL,
};

struct command_converter
{
    command operator()(const std::string& value) const
    {
        if (value == "add")
        {
            return command::ADD;
        }
        else if (value == "mul")
        {
            return command::MUL;
        }
        throw conversion_error();
    }
};

struct settings
{
    bool m_verbose;
    command m_command;
    bool m_show_steps;
    std::vector<int> m_values;

    settings()
        : m_verbose(false)
        , m_command(command::INVALID)
        , m_show_steps(false)
        , m_values()
    {
        // noop
    }
};

} // namespace

TEST(subparser_test, test_storing)
{
    storing_parser<settings> parser;
    parser.add_switch({ "-v", "--verbose" }).set_type_and_storage(&settings::m_verbose);
    auto subparsers = parser.add_subparsers("command");
    subparsers.set_type<command>().set_convert_function(command_converter()).set_storage(&settings::m_command);

    auto add_parser = subparsers.add_parser("add");
    add_parser.add_switch({ "-s", "--show-steps" }).set_type_and_storage(&settings::m_show_steps);
    add_parser.add_positional("values").set_max_count_unlimited().set_type_and_storage(&settings::m_values);

    auto mul_parser = subparsers.add_parser("mul");
    mul_parser.add_switch({ "-s", "--show-steps" }).set_type_and_storage(&settings::m_show_steps);
    mul_parser.add_positional("values").set_max_count_unlimited().set_type_and_storage(&settings::m_values);

    argument_table args1("appname", { "-v", "add", "2", "4", "18" });

    settings settings1;
    parser.parse(args1, settings1);
    ASSERT_EQ(true, settings1.m_verbose);
    ASSERT_EQ(command::ADD, settings1.m_command);
    ASSERT_EQ(false, settings1.m_show_steps);
    ASSERT_EQ(std::size_t(3), settings1.m_values.size());
    ASSERT_EQ(int(2), settings1.m_values[0]);
    ASSERT_EQ(int(4), settings1.m_values[1]);
    ASSERT_EQ(int(18), settings1.m_values[2]);

    argument_table args2("appname", { "mul", "-s", "-3", "102", "12" });

    settings settings2;
    parser.parse(args2, settings2);
    ASSERT_EQ(false, settings2.m_verbose);
    ASSERT_EQ(command::MUL, settings2.m_command);
    ASSERT_EQ(true, settings2.m_show_steps);
    ASSERT_EQ(std::size_t(3), settings2.m_values.size());
    ASSERT_EQ(int(-3), settings2.m_values[0]);
    ASSERT_EQ(int(102), settings2.m_values[1]);
    ASSERT_EQ(int(12), settings2.m_values[2]);

    argument_table args3("appname", { "xxx", "102", "12" });

    settings settings3;
    ASSERT_THROW(parser.parse(args3, settings3), parser_error);
}

TEST(subparser_test, test_errors)
{
    parser parser1;
    parser1.add_positional("values");
    ASSERT_THROW(parser1.add_subparsers("values"), invalid_argument_name);
    ASSERT_THROW(parser1.add_subparsers("comands"), subparser_positional_conflict);

    parser parser2;
    parser2.add_subparsers("values");
    ASSERT_THROW(parser2.add_positional("values"), invalid_argument_name);
    ASSERT_THROW(parser2.add_positional("positionals"), subparser_positional_conflict);
    ASSERT_THROW(parser2.add_subparsers("subparsers"), subparser_positional_conflict);

    parser parser3;
    auto subparsers = parser3.add_subparsers("commands");
    subparsers.add_parser("add");
    subparsers.add_parser("mul");

    argument_table args1("appname", {});
    ASSERT_THROW(parser3.parse(args1), parser_error);

    argument_table args2("appname", { "sub" });
    ASSERT_THROW(parser3.parse(args2), parser_error);
}

} // namespace args
} // namespace oct
