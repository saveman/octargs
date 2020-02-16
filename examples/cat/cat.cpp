#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "octargs.hpp"

namespace oct_args_examples
{

namespace
{

static const std::string STANDARD_INPUT_NAME("-");

}

class execution_error : public std::exception
{
public:
    execution_error(const std::string& message)
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
    cat_app_settings()
        : m_print_line_ends(false)
        , m_print_line_numbers(false)
        , m_input_names()
    {
        // noop
    }

    bool m_print_line_ends;
    bool m_print_line_numbers;
    std::vector<std::string> m_input_names;
};

class cat_app_engine
{
public:
    cat_app_engine(const cat_app_settings& settings)
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
    cat_app()
    {
        // noop
    }

    int run(int argc, char* argv[])
    {
        try
        {
            // cat [OPTION]... [FILE]...
            //
            // Concatenate FILE(s) to standard output.
            // With no FILE, or when FILE is -, read standard input.

            oct::args::storing_parser<cat_app_settings> arg_parser;

            arg_parser.add_switch({ "--help" }); // TODO: remove

            arg_parser.add_switch({ "-E", "--show-ends" }).set_storage_location(&cat_app_settings::m_print_line_ends);
            // TODO: .set_description("display $ at end of each line");
            arg_parser.add_switch({ "-n", "--number" }).set_storage_location(&cat_app_settings::m_print_line_numbers);
            // TODO: .set_description("number all output lines");
            arg_parser.add_positional("FILES", false, true).set_storage_location(&cat_app_settings::m_input_names);

            cat_app_settings settings;

            arg_parser.parse(argc, argv, settings);

            // TODO: (planned) - change when defaults values support is ready
            if (settings.m_input_names.empty())
            {
                settings.m_input_names.push_back(STANDARD_INPUT_NAME);
            }

            cat_app_engine(settings).execute();
        }
        catch (const oct::args::parse_exception& exc)
        {
            std::cerr << "Invalid arguments: " << exc.what() << std::endl;
            print_usage(std::cerr);
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
    void print_usage(std::ostream& /*os*/)
    {
        // TODO
    }
};

} // namespace oct_args_examples

int main(int argc, char* argv[])
{
    return oct_args_examples::cat_app().run(argc, argv);
}
