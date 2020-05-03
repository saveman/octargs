#include "gtest/gtest.h"

#include <sstream>
#include <string>

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

TEST(parser_usage_test, test_header_footer)
{
    parser parser;
    parser.set_usage_oneliner("This is oneliner");
    parser.set_usage_header("This is header\n  Second line of header!");
    parser.set_usage_footer("This is footer\n  Second line of footer!\nThird line of footer");

    std::ostringstream out_ostream;

    out_ostream << parser.get_usage();

    const std::vector<std::string> EXPECTED_RESULT_LINES = {
        "Usage: (no arguments available)",
        "",
        "This is oneliner",
        "",
        "This is header",
        "  Second line of header!",
        "",
        "This is footer",
        "  Second line of footer!",
        "Third line of footer",
    };

    std::ostringstream expected_stream;
    for (auto& line : EXPECTED_RESULT_LINES)
    {
        expected_stream << line << std::endl;
    }

    ASSERT_EQ(expected_stream.str(), out_ostream.str());
}

TEST(parser_usage_test, test_example)
{
    parser parser;
    parser.set_usage_oneliner("ONELINER");
    parser.set_usage_header("HEADER");
    parser.set_usage_footer("FOOTER");
    parser.add_switch({ "-n" }).set_description("the n!");
    parser.add_valued({ "-k" }).set_description("the k!");
    parser.add_valued({ "-p" })
        .set_description("the p!")
        .set_min_count(1)
        .set_max_count(3)
        .set_default_values({ "a", "b" })
        .set_allowed_values({ "a", "b", "c", "de", "ef" });
    parser.add_switch({ "-a", "-b" }).set_description("the ab!");
    parser.add_valued({ "-x", "--thex" }).set_description("the\nmulti\nline!").set_min_count(1);
    parser.add_valued({ "-y", "--they" })
        .set_description("the\nmulti\nline!")
        .set_min_count(2)
        .set_max_count(5)
        .set_value_name("VAL");
    parser.add_switch({ "--ala", "--tola" }).set_description("alatola check my name");
    parser.add_positional("PATTERN").set_description("Pattern to find").set_min_count(1);
    parser.add_positional("FILES").set_description("Files to process").set_max_count_unlimited();

    std::ostringstream out_ostream;

    out_ostream << parser.get_usage();

    const std::vector<std::string> EXPECTED_RESULT_LINES = {
        "Usage: <OPTIONS>... <PATTERN> [FILES]...",
        "",
        "ONELINER",
        "",
        "HEADER",
        "",
        "Optional arguments:",
        "  -n                    the n!",
        "  -k                    the k!",
        "  -p                    the p!",
        "                          [max: 3]",
        "                          [default: a, b]",
        "                          [allowed: a, b, c, de, ef]",
        "  -a, -b                the ab!",
        "  -x,    --thex         the",
        "                          multi",
        "                          line!",
        "                          [required]",
        "  -y,    --they=VAL     the",
        "                          multi",
        "                          line!",
        "                          [min: 2] [max: 5]",
        "         --ala, --tola  alatola check my name",
        "",
        "Positional arguments:",
        "  PATTERN  Pattern to find",
        "             [required]",
        "  FILES    Files to process",
        "             [unlimited]",
        "",
        "FOOTER",
    };
    std::ostringstream expected_stream;
    for (auto& line : EXPECTED_RESULT_LINES)
    {
        expected_stream << line << std::endl;
    }

    ASSERT_EQ(expected_stream.str(), out_ostream.str());
}

TEST(parser_usage_test, test_example_wchar)
{
    wparser parser;
    parser.set_usage_oneliner(L"ONELINER");
    parser.set_usage_header(L"HEADER");
    parser.set_usage_footer(L"FOOTER");
    parser.add_switch({ L"-n" }).set_description(L"the n!");
    parser.add_valued({ L"-k" }).set_description(L"the k!");
    parser.add_switch({ L"-a", L"-b" }).set_description(L"the ab!");
    parser.add_valued({ L"-x", L"--thex" }).set_description(L"the\nmulti\nline!");
    parser.add_valued({ L"-y", L"--they" })
        .set_description(L"the\nmulti\nline!")
        .set_min_count(2)
        .set_max_count(5)
        .set_value_name(L"VAL");
    parser.add_switch({ L"--ala", L"--tola" }).set_description(L"alatola check my name");
    parser.add_positional(L"FILES").set_description(L"Files to process").set_max_count_unlimited();

    std::wostringstream out_ostream;

    out_ostream << parser.get_usage();

    const std::vector<std::wstring> EXPECTED_RESULT_LINES = {
        L"Usage: <OPTIONS>... [FILES]...",
        L"",
        L"ONELINER",
        L"",
        L"HEADER",
        L"",
        L"Optional arguments:",
        L"  -n                    the n!",
        L"  -k                    the k!",
        L"  -a, -b                the ab!",
        L"  -x,    --thex         the",
        L"                          multi",
        L"                          line!",
        L"  -y,    --they=VAL     the",
        L"                          multi",
        L"                          line!",
        L"                          [min: 2] [max: 5]",
        L"         --ala, --tola  alatola check my name",
        L"",
        L"Positional arguments:",
        L"  FILES  Files to process",
        L"           [unlimited]",
        L"",
        L"FOOTER",
    };
    std::wostringstream expected_stream;
    for (auto& line : EXPECTED_RESULT_LINES)
    {
        expected_stream << line << std::endl;
    }

    ASSERT_TRUE(expected_stream.str() == out_ostream.str());
}

TEST(parser_usage_test, test_subparsers)
{
    parser parser;
    parser.set_usage_oneliner("ONELINER");
    parser.set_usage_header("HEADER");
    parser.set_usage_footer("FOOTER");
    parser.add_exclusive({ "--help" });
    parser.add_switch({ "--verbose", "-v" }).set_description("print verbose output");

    auto subparsers = parser.add_subparsers("command");

    auto get_parser = subparsers.add_parser("get");
    get_parser.set_usage_oneliner("get file");
    get_parser.add_exclusive({ "--help" });
    get_parser.add_valued({ "--src" }).set_value_name("uri");
    get_parser.add_valued({ "--dst" }).set_value_name("filepath");

    auto put_parser = subparsers.add_parser("put");
    put_parser.set_usage_oneliner("put file");
    put_parser.add_exclusive({ "--help" });
    put_parser.add_valued({ "--src" }).set_value_name("filepath");
    put_parser.add_valued({ "--dst" }).set_value_name("uri");

    std::ostringstream out_ostream;

    out_ostream << parser.get_usage();

    const std::vector<std::string> EXPECTED_RESULT_LINES = {
        "Usage: [OPTIONS] <command> [SUBOPTS]",
        "",
        "ONELINER",
        "",
        "HEADER",
        "",
        "Optional arguments:",
        "      --help     ",
        "  -v, --verbose  print verbose output",
        "",
        "command:",
        "  get  get file",
        "  put  put file",
        "",
        "FOOTER",
    };
    std::ostringstream expected_stream;
    for (auto& line : EXPECTED_RESULT_LINES)
    {
        expected_stream << line << std::endl;
    }

    ASSERT_EQ(expected_stream.str(), out_ostream.str());
}

TEST(parser_usage_test, test_group)
{
    parser parser;
    parser.set_usage_oneliner("ONELINER");
    parser.set_usage_header("HEADER");
    parser.set_usage_footer("FOOTER");
    parser.add_switch("--debug").set_description("debug output");
    parser.add_positional("PATTERN").set_description("Pattern to find").set_min_count(1);
    auto group1 = parser.add_group("Group 1");
    group1.add_switch("--verbose").set_description("an option");
    group1.add_switch({ "-n" }).set_description("the n!");
    group1.add_valued("-k").set_description("the k!");
    group1.add_switch({ "-a", "-b" }).set_description("the ab!");
    group1.add_valued({ "-x", "--thex" }).set_description("the\nmulti\nline!").set_min_count(1);
    group1.add_switch({ "--ala", "--tola" }).set_description("alatola check my name");
    auto group2 = parser.add_group("Group 2");
    group2.set_description("This is a group with single line description");
    group2.add_valued({ "--other" }).set_value_name("value").set_description("other value");
    group2.add_positional("VALUES").set_max_count(2).set_description("Some values").set_value_name("VALS");
    auto group3 = parser.add_group("Group 3");
    group3.set_description("This is a group\nwith multiline description.\nSo it is split.");
    group3.add_valued({ "--another" }).set_value_name("value").set_description("another value");

    std::ostringstream out_ostream;

    out_ostream << parser.get_usage();

    const std::vector<std::string> EXPECTED_RESULT_LINES = {
        "Usage: <OPTIONS>... <PATTERN> [VALS]...",
        "",
        "ONELINER",
        "",
        "HEADER",
        "",
        "Optional arguments:",
        "  --debug  debug output",
        "",
        "Positional arguments:",
        "  PATTERN  Pattern to find",
        "             [required]",
        "",
        "Group 1:",
        "         --verbose      an option",
        "  -n                    the n!",
        "  -k                    the k!",
        "  -a, -b                the ab!",
        "  -x,    --thex         the",
        "                          multi",
        "                          line!",
        "                          [required]",
        "         --ala, --tola  alatola check my name",
        "",
        "Group 2:",
        "  This is a group with single line description",
        "  --other=value  other value",
        "  VALS           Some values",
        "                   [max: 2]",
        "",
        "Group 3:",
        "  This is a group",
        "  with multiline description.",
        "  So it is split.",
        "  --another=value  another value",
        "",
        "FOOTER",
    };
    std::ostringstream expected_stream;
    for (auto& line : EXPECTED_RESULT_LINES)
    {
        expected_stream << line << std::endl;
    }

    ASSERT_EQ(expected_stream.str(), out_ostream.str());
}

TEST(parser_usage_test, test_custom_dictionary)
{
    using dictionary_type = custom_dictionary<char>;

    auto dictionary = std::make_shared<dictionary_type>(dictionary_type::init_mode::WITH_DEFAULTS);
    dictionary->set_usage_literal(dictionary_type::usage_literal::LEAD, "Użycie");
    dictionary->set_usage_literal(dictionary_type::usage_literal::DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME, "Wartości");
    dictionary->set_usage_literal(dictionary_type::usage_literal::DEFAULT_NAMED_ARGUMENTS_GROUP_NAME, "Opcje");
    dictionary->set_usage_literal(dictionary_type::usage_literal::DECORATOR_MAX_COUNT_UNLIMITED, "nielimitowany");

    parser parser(dictionary);
    parser.set_usage_oneliner("ONELINER");
    parser.set_usage_header("HEADER");
    parser.set_usage_footer("FOOTER");
    parser.add_switch({ "-n" }).set_description("the n!");
    parser.add_valued({ "-p" })
        .set_description("the p!")
        .set_min_count(1)
        .set_max_count(3)
        .set_default_values({ "a", "b" })
        .set_allowed_values({ "a", "b", "c", "de", "ef" });
    parser.add_positional("PATTERN").set_description("Pattern to find").set_min_count(1);
    parser.add_positional("FILES").set_description("Files to process").set_max_count_unlimited();

    std::ostringstream out_ostream;

    out_ostream << parser.get_usage();

    const std::vector<std::string> EXPECTED_RESULT_LINES = {
        "Użycie: <OPTIONS>... <PATTERN> [FILES]...",
        "",
        "ONELINER",
        "",
        "HEADER",
        "",
        "Opcje:",
        "  -n  the n!",
        "  -p  the p!",
        "        [max: 3]",
        "        [default: a, b]",
        "        [allowed: a, b, c, de, ef]",
        "",
        "Wartości:",
        "  PATTERN  Pattern to find",
        "             [required]",
        "  FILES    Files to process",
        "             [nielimitowany]",
        "",
        "FOOTER",
    };
    std::ostringstream expected_stream;
    for (auto& line : EXPECTED_RESULT_LINES)
    {
        expected_stream << line << std::endl;
    }

    ASSERT_EQ(expected_stream.str(), out_ostream.str());
}

} // namespace args
} // namespace oct
