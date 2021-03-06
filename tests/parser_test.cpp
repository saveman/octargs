#include "gtest/gtest.h"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

namespace
{

enum class format_code
{
    UNKNOWN,
    HEX,
    DEC,
};

class format_code_converter
{
public:
    format_code operator()(const std::string& value_str) const
    {
        if (value_str == "hex")
        {
            return format_code::HEX;
        }
        else if (value_str == "dec")
        {
            return format_code::DEC;
        }
        throw conversion_error_ex<char>(value_str);
    }
};

inline std::ostream& operator<<(std::ostream& os, format_code code)
{
    return os << static_cast<int>(code);
}

} // namespace

TEST(parser_test, test_argc_argv_parse)
{
    char app[] { "appname" };
    char arg1[] { "-v" };
    char arg2[] { "argument1" };
    char arg3[] { "argument2" };
    char* argv[] = { app, arg1, arg2, arg3, nullptr };
    int argc = 4;

    parser parser;
    parser.add_switch({ "-v" });
    parser.add_switch({ "--help" });
    parser.add_positional("values").set_max_count_unlimited();

    auto results = parser.parse(argc, argv);
    ASSERT_EQ(std::string("appname"), results.get_app_name());
    ASSERT_TRUE(results.has_value("-v"));
    ASSERT_TRUE(!results.has_value("--help"));
    ASSERT_EQ(std::size_t(2), results.get_count("values"));
    ASSERT_EQ(std::string("argument1"), results.get_values("values")[0]);
    ASSERT_EQ(std::string("argument2"), results.get_values("values")[1]);
}

TEST(parser_test, test_argument_table_parse)
{
    argument_table args("appname", { "-v", "argument1", "argument2" });

    parser parser;
    parser.add_switch({ "-v" });
    parser.add_switch({ "--help" });
    parser.add_positional("values").set_max_count_unlimited();

    auto results = parser.parse(args);
    ASSERT_TRUE(results.has_value("-v"));
    ASSERT_TRUE(!results.has_value("--help"));
    ASSERT_EQ(std::size_t(2), results.get_count("values"));
    ASSERT_EQ(std::string("argument1"), results.get_values("values")[0]);
    ASSERT_EQ(std::string("argument2"), results.get_values("values")[1]);
}

TEST(parser_test, test_invalid_results_arg)
{
    argument_table args("appname", { "-v" });

    parser parser;
    parser.add_switch({ "-v" });

    auto results = parser.parse(args);
    ASSERT_TRUE(results.has_value("-v"));
    ASSERT_THROW(results.has_value("-a"), unknown_argument);
}

TEST(parser_test, test_unknown_argument)
{
    argument_table args("appname", { "-v", "-a" });

    parser parser;
    parser.add_switch({ "-v" });

    ASSERT_THROW(parser.parse(args), parser_error);
}

TEST(parser_test, test_custom_type)
{
    parser parser;

    parser.add_valued({ "--format" });
    parser.add_valued({ "--multi" }).set_max_count_unlimited();

    auto results0 = parser.parse(argument_table("appname", {}));
    ASSERT_EQ(std::size_t(0), results0.get_count("--format"));
    ASSERT_THROW(results0.get_first_value("--format"), std::logic_error);
    ASSERT_EQ("aaa", (results0.get_first_value("--format", "aaa")));
    ASSERT_THROW((results0.get_first_value_as<format_code, format_code_converter>("--format")), std::logic_error);
    ASSERT_EQ(format_code::UNKNOWN,
        (results0.get_first_value_as<format_code, format_code_converter>("--format", format_code::UNKNOWN)));

    auto results1 = parser.parse(argument_table("appname", { "--format=hex" }));
    ASSERT_EQ(std::size_t(1), results1.get_count("--format"));
    ASSERT_EQ(std::string("hex"), results1.get_first_value("--format"));
    ASSERT_EQ(std::string("hex"), results1.get_first_value("--format", "dec"));
    ASSERT_EQ(format_code::HEX, (results1.get_first_value_as<format_code, format_code_converter>("--format")));
    ASSERT_EQ(format_code::HEX,
        (results1.get_first_value_as<format_code, format_code_converter>("--format", format_code::DEC)));

    auto results2 = parser.parse(argument_table("appname", { "--format=dec" }));
    ASSERT_EQ(std::size_t(1), results2.get_count("--format"));
    ASSERT_EQ(std::string("dec"), results2.get_first_value("--format"));
    auto format2 = results2.get_first_value_as<format_code, format_code_converter>("--format");
    ASSERT_EQ(format_code::DEC, format2);

    auto results3 = parser.parse(argument_table("appname", { "--format=aaa" }));
    try
    {
        results3.get_values_as<format_code, format_code_converter>("--format");
        ASSERT_TRUE(false);
    }
    catch (const conversion_error&)
    {
        // ignore
    }

    auto results4
        = parser.parse(argument_table("appname", { "--multi=dec", "--multi", "hex", "--multi", "dec", "--multi=hex" }));
    ASSERT_EQ(std::size_t(0), results4.get_count("--format"));
    ASSERT_EQ(std::size_t(4), results4.get_count("--multi"));
    ASSERT_EQ(std::string("dec"), results4.get_values("--multi")[0]);
    ASSERT_EQ(std::string("hex"), results4.get_values("--multi")[1]);
    ASSERT_EQ(std::string("dec"), results4.get_values("--multi")[2]);
    ASSERT_EQ(std::string("hex"), results4.get_values("--multi")[3]);
    auto format4 = results4.get_first_value_as<format_code, format_code_converter>("--multi");
    ASSERT_EQ(format_code::DEC, format4);
    auto formats4 = results4.get_values_as<format_code, format_code_converter>("--multi");
    ASSERT_EQ(std::size_t(4), formats4.size());
    ASSERT_EQ(format_code::DEC, formats4[0]);
    ASSERT_EQ(format_code::HEX, formats4[1]);
    ASSERT_EQ(format_code::DEC, formats4[2]);
    ASSERT_EQ(format_code::HEX, formats4[3]);
}

} // namespace args
} // namespace oct
