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

class cat_app
{
public:
    cat_app()
        : m_line_number(0)
        , m_print_line_ends(false)
        , m_print_line_numbers(false)
    {
        // noop
    }

    int run(int argc, char* argv[])
    {
        m_line_number = 1;
        m_print_line_ends = false;
        m_print_line_numbers = false;

        try
        {
            // cat [OPTION]... [FILE]...
            //
            // Concatenate FILE(s) to standard output.
            // With no FILE, or when FILE is -, read standard input.

            oct::args::parser arg_parser;

            arg_parser.add_switch({ "-E", "--show-ends" }); // TODO: .set_description("display $ at end of each line");
            arg_parser.add_switch({ "-n", "--number" }); // TODO: .set_description("number all output lines");
            arg_parser.add_positional("FILES", false, true);

            auto results = arg_parser.parse(argc, argv);

            // TODO: refactor when 'value storage API is ready'
            m_print_line_ends = results.has_value("--show-ends");
            m_print_line_numbers = results.has_value("--number");

            if (results.count("FILES") > 0)
            {
                process_inputs(results.values("FILES"));
            }
            else
            {
                process_inputs({ STANDARD_INPUT_NAME });
            }
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
    static const int LINE_NUMBER_LENGTH = 6;

    void process_inputs(const std::vector<std::string>& input_names)
    {
        for (const auto& input_name : input_names)
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

    void print_usage(std::ostream& /*os*/)
    {
        // TODO
    }

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
        if (m_print_line_numbers)
        {
            std::cout << std::setw(LINE_NUMBER_LENGTH) << (m_line_number++) << "  ";
        }

        std::cout << line;

        if (m_print_line_ends)
        {
            std::cout << '$';
        }

        std::cout << "\n";
    }

    int m_line_number;
    bool m_print_line_ends;
    bool m_print_line_numbers;
};

} // namespace oct_args_examples

int main(int argc, char* argv[])
{
    return oct_args_examples::cat_app().run(argc, argv);
}
