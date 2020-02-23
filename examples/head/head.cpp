#include <cstdint>
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

class parse_error : public std::runtime_error
{
public:
    parse_error(const std::string& message)
        : std::runtime_error(message) {
            // noop }
        };
};

class execution_error : public std::runtime_error
{
public:
    explicit execution_error(const std::string& message)
        : std::runtime_error(message)
    {
        // noop
    }
};

class head_app
{
public:
    head_app()
    {
        set_defaults();
    }

    int run(int argc, char* argv[])
    {
        set_defaults();

        try
        {
            oct::args::parser arg_parser;

            arg_parser.set_info_header(std::string("Print the first ") + std::to_string(DEFAULT_LINES_LIMIT)
                + "lines of each FILE to standard output.\n\n"
                  "With no FILE, or when FILE is -, read standard input.");
            arg_parser.set_info_footer("NUM may have a multiplier suffix:\n"
                                       "b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n"
                                       "GB 1000*1000*1000, G 1024*1024*1024");

            arg_parser.add_exclusive({ "--help" }).set_description("display this help and exit");
            arg_parser.add_valued({ "-b", "--bytes" }).set_description("number of bytes to print").set_value_name("B");
            arg_parser.add_valued({ "-n", "--lines" }).set_description("number of lines to print").set_value_name("N");
            arg_parser.add_switch({ "-h", "--header" }).set_description("print header with input name");
            arg_parser.add_switch({ "-z", "--zero-terminated" }).set_description("line delimiter is NUL, not newline");
            arg_parser.add_positional("FILES").set_description("files to process").set_max_count_unlimited();

            auto results = arg_parser.parse(argc, argv);

            if (results.has_value("--help"))
            {
                std::cout << arg_parser.usage() << std::endl;
                return EXIT_SUCCESS;
            }

            if (results.has_value("--zero-terminated"))
            {
                m_line_terminator = '\0';
            }
            m_print_header = results.has_value("--header");
            if (results.has_value("--bytes"))
            {
                m_bytes_limit = parse_limit(results.values("--bytes")[0]);
            }
            if (results.has_value("--lines"))
            {
                m_lines_limit = parse_limit(results.values("--lines")[0]);
            }
            if ((m_bytes_limit < 0) && (m_lines_limit < 0))
            {
                m_lines_limit = DEFAULT_LINES_LIMIT;
            }

            if (results.count("FILES") == 0)
            {
                process_inputs({ STANDARD_INPUT_NAME });
            }
            else
            {
                process_inputs(results.values("FILES"));
            }
        }
        catch (const oct::args::parser_error_ex<char>& exc)
        {
            std::cerr << "Argument parsing error near: " << exc.name() << " " << exc.value() << std::endl;
            std::cerr << "Run " << argv[0] << " --help to see usage information" << std::endl;
            return EXIT_FAILURE;
        }
        catch (const parse_error& exc)
        {
            std::cerr << "Argument parsing error: " << exc.what() << std::endl;
            std::cerr << "Run " << argv[0] << " --help to see usage information" << std::endl;
            return EXIT_FAILURE;
        }
        catch (const execution_error& exc)
        {
            std::cerr << "Execution failed: " << exc.what() << std::endl;
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
    static const long long DEFAULT_LINES_LIMIT = 10;

    static const int LINE_NUMBER_LENGTH = 6;

    static long long parse_limit(const std::string& value_str)
    {
        long long limit = -1;

        std::size_t parsed_chars = 0;
        try
        {
            limit = std::stoll(value_str, &parsed_chars, 0);
            if (limit < 0)
            {
                throw parse_error("Negative limit is not supported");
            }

            std::string suffix = value_str.substr(parsed_chars);

            static std::map<std::string, long long> SUFFIX_VALUES = {
                { "", 1 },
                { "b", 512 },
                { "kB", 1000 },
                { "KB", 1000 },
                { "K", 1024 },
                { "MB", 1000 * 1000 },
                { "M", 1024 * 1024 },
                { "GB", 1000 * 1000 * 1000 },
                { "G", 1000 * 1000 * 1000 },
            };

            auto iter = SUFFIX_VALUES.find(suffix);
            if (iter != SUFFIX_VALUES.end())
            {
                limit *= iter->second;
            }
            else
            {
                throw parse_error("Invalid suffix in limit");
            }
        }
        catch (const std::exception& exc)
        {
            throw parse_error("Invalid limit value");
        }

        return limit;
    }

    void set_defaults()
    {
        m_line_terminator = '\n';
        m_print_header = false;
        m_bytes_limit = -1;
        m_lines_limit = -1;
        m_first_input = true;
    }

    void process_inputs(const std::vector<std::string>& input_names)
    {
        for (const auto& input_name : input_names)
        {
            if (input_name == STANDARD_INPUT_NAME)
            {
                head_istream("standard input", std::cin);
            }
            else
            {
                head_file(input_name);
            }

            m_first_input = false;
        }
    }

    void head_file(const std::string& file_name) const
    {
        std::ifstream input_file(file_name, std::ifstream::binary);
        if (!input_file.is_open())
        {
            throw execution_error(std::string("Cannot open file: ") + file_name);
        }

        head_istream(file_name, input_file);
    }

    void head_istream(const std::string& name, std::istream& input_stream) const
    {
        if (m_print_header)
        {
            if (!m_first_input)
            {
                std::cout << "\n";
            }
            std::cout << "==> " << name << " <==\n";
        }

        if (input_stream.eof())
        {
            // already at end - exit with success
            return;
        }

        // not optimized, this is just an example
        long long bytes_count = 0;
        long long lines_count = 0;

        while (((m_bytes_limit < 0) || (bytes_count < m_bytes_limit))
            && ((m_lines_limit < 0) || (lines_count < m_lines_limit)))
        {
            char c = 0;

            if (!input_stream.get(c))
            {
                if (!input_stream.eof())
                {
                    throw execution_error(std::string("Cannot print contents of: ") + name);
                }
                break;
            }

            ++bytes_count;
            if (c == m_line_terminator)
            {
                ++lines_count;
                if (m_line_terminator == '\0')
                {
                    c = '\n';
                }
            }

            std::cout << c;
        }
    }

    bool m_print_header;
    char m_line_terminator;
    long long m_bytes_limit;
    long long m_lines_limit;
    bool m_first_input;
};

} // namespace oct_args_examples

int main(int argc, char* argv[])
{
    return oct_args_examples::head_app().run(argc, argv);
}
