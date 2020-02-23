#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "octargs/octargs.hpp"

namespace oct_args_examples
{

namespace
{

static const std::string STANDARD_INPUT_NAME("-");

}

class execution_error : public std::exception
{
public:
    explicit execution_error(const std::string& message)
        : m_message(message)
    {
        // noop
    }

    virtual const char* what() const noexcept
    {
        return m_message.c_str();
    }

private:
    std::string m_message;
};

struct cat_app_settings
{
    explicit cat_app_settings()
        : m_help_requested(false)
        , m_print_line_ends(false)
        , m_print_line_numbers(false)
        , m_input_names()
    {
        // noop
    }

    bool m_help_requested;
    bool m_print_line_ends;
    bool m_print_line_numbers;
    std::vector<std::string> m_input_names;
};

class cat_app_engine
{
public:
    explicit cat_app_engine(const cat_app_settings& settings)
        : m_settings(settings)
        , m_line_number(0)
    {
        // noop
    }

    void execute()
    {
        m_line_number = 1;

        for (const auto& input_name : m_settings.m_input_names)
        {
            if (input_name == STANDARD_INPUT_NAME)
            {
                cat_stream_contents("standard input", std::cin);
            }
            else
            {
                cat_file(input_name);
            }
        }
    }

private:
    static const int LINE_NUMBER_LENGTH = 6;

    void cat_file(const std::string& file_name)
    {
        std::ifstream input_file(file_name);
        if (!input_file.is_open())
        {
            throw execution_error(std::string("Cannot open file: ") + file_name);
        }

        cat_stream_contents(file_name, input_file);
    }

    void cat_stream_contents(const std::string& name, std::istream& input_stream)
    {
        std::string line;

        if (input_stream.eof())
        {
            // already at end - exit with success
            return;
        }

        while (std::getline(input_stream, line))
        {
            cat_line(line);
        }

        if (!input_stream.eof())
        {
            throw execution_error(std::string("Cannot print contents of: ") + name);
        }
    }

    void cat_line(const std::string& line)
    {
        if (m_settings.m_print_line_numbers)
        {
            std::cout << std::setw(LINE_NUMBER_LENGTH) << (m_line_number++) << "  ";
        }

        std::cout << line;

        if (m_settings.m_print_line_ends)
        {
            std::cout << '$';
        }

        std::cout << "\n";
    }

    cat_app_settings m_settings;
    int m_line_number;
};

class cat_app
{
public:
    cat_app(int argc, char* argv[])
        : m_input_args(argc, argv)
    {
        // noop
    }

    int run()
    {
        oct::args::storing_parser<cat_app_settings> arg_parser;

        try
        {
            arg_parser
                .set_info_header("Concatenate FILE(s) to standard output.\n\n"
                                 "With no FILE, or when FILE is -, read standard input.")
                .set_info_footer("Examples:\n"
                                 "cat f - g  Output f's contents, then standard input, then g's contents.\n"
                                 "cat        Copy standard input to standard output.");
            arg_parser.add_exclusive({ "--help" })
                .set_description("show help information")
                .set_type_and_storage(&cat_app_settings::m_help_requested);
            arg_parser.add_switch({ "-E", "--show-ends" })
                .set_description("display $ at end of each line")
                .set_type_and_storage(&cat_app_settings::m_print_line_ends);
            arg_parser.add_switch({ "-n", "--number" })
                .set_description("number all output lines")
                .set_type_and_storage(&cat_app_settings::m_print_line_numbers);

            arg_parser.add_positional("FILES")
                .set_max_count_unlimited()
                .set_default_value(STANDARD_INPUT_NAME)
                .set_description("files to concatenate")
                .set_type_and_storage(&cat_app_settings::m_input_names);

            cat_app_settings settings;

            arg_parser.parse(m_input_args, settings);

            if (settings.m_help_requested)
            {
                std::cout << arg_parser.usage() << std::endl;
                return EXIT_SUCCESS;
            }
            cat_app_engine(settings).execute();
        }
        catch (const oct::args::parser_error_ex<char>& exc)
        {
            std::cerr << "Argument parsing error near: " << exc.name() << " " << exc.value() << std::endl;
            std::cerr << "Run " << m_input_args.get_app_name() << " --help to see usage information" << std::endl;
            return EXIT_FAILURE;
        }
        catch (const std::exception& exc)
        {
            std::cerr << "FATAL ERROR: " << exc.what() << std::endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

private:
    oct::args::argument_table m_input_args;
};

} // namespace oct_args_examples

int main(int argc, char* argv[])
{
    return oct_args_examples::cat_app(argc, argv).run();
}
