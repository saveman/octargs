#include "test_fixture.hpp"

#include "../include/octargs/octargs.hpp"

#include <sstream>
#include <string>

namespace oct
{
namespace args
{

class parser_usage_test : public test_fixture
{
private:
    void test_header_footer()
    {
        parser parser;
        parser.set_info_header("This is header\n  Second line of header!");
        parser.set_info_footer("This is footer\n  Second line of footer!\nThird line of footer");

        std::ostringstream out_ostream;

        out_ostream << parser.usage();

        const std::vector<std::string> EXPECTED_RESULT_LINES = {
            "Usage: --",
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

        CPPUNIT_ASSERT_EQUAL(expected_stream.str(), out_ostream.str());
    }

    void test_example()
    {
        parser parser;
        parser.set_info_header("HEADER");
        parser.set_info_footer("FOOTER");
        parser.add_switch({ "-n" }).set_description("the n!");
        parser.add_valued({ "-k" }).set_description("the k!");
        parser.add_switch({ "-a", "-b" }).set_description("the ab!");
        parser.add_valued({ "-x", "--thex" }).set_description("the\nmulti\nline!");
        parser.add_valued({ "-y", "--they" })
            .set_description("the\nmulti\nline!")
            .set_min_count(2)
            .set_max_count(5)
            .set_value_name("VAL");
        parser.add_switch({ "--ala", "--tola" }).set_description("alatola check my name");
        parser.add_positional("FILES").set_description("Files to process").set_max_count_unlimited();

        std::ostringstream out_ostream;

        out_ostream << parser.usage();

        const std::vector<std::string> EXPECTED_RESULT_LINES = {
            "Usage: [OPTIONS]... [FILES]...",
            "",
            "HEADER",
            "",
            "Options:",
            "  -n                    the n!",
            "  -k                    the k!",
            "  -a, -b                the ab!",
            "  -x,    --thex         the",
            "                          multi",
            "                          line!",
            "  -y,    --they=VAL     the",
            "                          multi",
            "                          line!",
            "                          [min: 2] [max: 5]",
            "         --ala, --tola  alatola check my name",
            "",
            "Arguments:",
            "  FILES  Files to process",
            "           [unlimited]",
            "",
            "FOOTER",
        };
        std::ostringstream expected_stream;
        for (auto& line : EXPECTED_RESULT_LINES)
        {
            expected_stream << line << std::endl;
        }

        CPPUNIT_ASSERT_EQUAL(expected_stream.str(), out_ostream.str());
    }

    void test_example_wchar()
    {
        wparser parser;
        parser.set_info_header(L"HEADER");
        parser.set_info_footer(L"FOOTER");
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

        out_ostream << parser.usage();

        const std::vector<std::wstring> EXPECTED_RESULT_LINES = {
            L"Usage: [OPTIONS]... [FILES]...",
            L"",
            L"HEADER",
            L"",
            L"Options:",
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
            L"Arguments:",
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

        CPPUNIT_ASSERT(expected_stream.str() == out_ostream.str());
    }

    CPPUNIT_TEST_SUITE(parser_usage_test);
    CPPUNIT_TEST(test_header_footer);
    CPPUNIT_TEST(test_example);
    CPPUNIT_TEST(test_example_wchar);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(parser_usage_test);

} // namespace args
} // namespace oct
