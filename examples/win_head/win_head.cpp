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
static const long long DEFAULT_LINES_LIMIT = 10;

} // namespace

class parse_error : public std::runtime_error
{
public:
    explicit parse_error(const std::string& message)
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

class limit_converter
{
public:
    long long operator()(const std::string& value_str)
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
        catch (const std::exception&)
        {
            throw parse_error("Invalid limit value");
        }

        return limit;
    }
};

struct settings
{
    bool m_help_requested;
    std::int64_t m_bytes_limit;
    std::int64_t m_lines_limit;
    bool m_print_header;
    bool m_zero_terminated;
    std::vector<std::string> m_files;

    settings()
        : m_help_requested(false)
        , m_bytes_limit(-1)
        , m_lines_limit(-1)
        , m_print_header(false)
        , m_zero_terminated(false)
        , m_files()
    {
        // noop
    }
};

class context
{
public:
    context()
        : m_settings()
        , m_first_input(true)
    {
        // noop
    }

    context(const settings& settings)
        : m_settings(settings)
        , m_first_input(true)
    {
        // noop
    }

    int64_t get_bytes_limit() const
    {
        return m_settings.m_bytes_limit;
    }

    int64_t get_lines_limit() const
    {
        if ((m_settings.m_bytes_limit < 0) && (m_settings.m_lines_limit < 0))
        {
            return DEFAULT_LINES_LIMIT;
        }
        return m_settings.m_lines_limit;
    }

    char get_line_terminator() const
    {
        return m_settings.m_zero_terminated ? '\0' : '\n';
    }

    std::vector<std::string> get_files() const
    {
        if (m_settings.m_files.empty())
        {
            return { STANDARD_INPUT_NAME };
        }
        return m_settings.m_files;
    }

    const settings& get_settings() const
    {
        return m_settings;
    }

    bool is_first_input() const
    {
        return m_first_input;
    }

    void clear_first_input()
    {
        m_first_input = false;
    }

private:
    settings m_settings;
    bool m_first_input;
};

class head_app
{
public:
    head_app()
    {
        // noop
    }

    int run(int argc, char* argv[])
    {
        try
        {
            using parser_type = oct::args::storing_parser<settings>;
            using dictionary_type = oct::args::custom_dictionary<parser_type::char_type>;

            auto dictionary = std::make_shared<dictionary_type>(dictionary_type::init_mode::WITH_DEFAULTS);
            dictionary->set_short_name_prefix("");
            dictionary->set_long_name_prefix("/");
            dictionary->set_value_separator_literal(":");

            parser_type arg_parser(dictionary);

            arg_parser.set_usage_oneliner(std::string("Print the first ") + std::to_string(DEFAULT_LINES_LIMIT)
                + "lines of each FILE to standard output.");
            arg_parser.set_usage_header("With no FILE, or when FILE is -, read standard input.");
            arg_parser.set_usage_footer("NUM may have a multiplier suffix:\n"
                                        "b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n"
                                        "GB 1000*1000*1000, G 1024*1024*1024");

            arg_parser.add_exclusive({ "/?" })
                .set_description("display this help and exit")
                .set_type_and_storage(&settings::m_help_requested);
            arg_parser.add_valued({ "/B", "/BYTES" })
                .set_description("number of bytes to print")
                .set_value_name("N")
                .set_type_and_storage(&settings::m_bytes_limit)
                .set_convert_function(limit_converter());
            arg_parser.add_valued({ "/N", "/LINES" })
                .set_description("number of lines to print")
                .set_value_name("N")
                .set_type_and_storage(&settings::m_lines_limit)
                .set_convert_function(limit_converter());
            arg_parser.add_switch({ "/H", "/HEADER" })
                .set_description("print header with input name")
                .set_type_and_storage(&settings::m_print_header);
            arg_parser.add_switch({ "/Z", "/ZERO", "/ZEROTERMINATED" })
                .set_description("line delimiter is NUL, not newline")
                .set_type_and_storage(&settings::m_zero_terminated);
            arg_parser.add_positional("FILES")
                .set_description("files to process")
                .set_max_count_unlimited()
                .set_type_and_storage(&settings::m_files);

            settings parsed_settings;
            arg_parser.parse(argc, argv, parsed_settings);

            m_context = context(parsed_settings);

            if (m_context.get_settings().m_help_requested)
            {
                std::cout << arg_parser.get_usage() << std::endl;
                return EXIT_SUCCESS;
            }

            process_inputs();
        }
        catch (const oct::args::parser_error_ex<char>& exc)
        {
            std::cerr << "Argument parsing error near: " << exc.get_name() << " " << exc.get_value() << std::endl;
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
    static const int LINE_NUMBER_LENGTH = 6;

    void process_inputs()
    {
        for (const auto& input_name : m_context.get_files())
        {
            if (input_name == STANDARD_INPUT_NAME)
            {
                head_istream("standard input", std::cin);
            }
            else
            {
                head_file(input_name);
            }

            m_context.clear_first_input();
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
        if (m_context.get_settings().m_print_header)
        {
            if (!m_context.is_first_input())
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

        while (((m_context.get_bytes_limit() < 0) || (bytes_count < m_context.get_bytes_limit()))
            && ((m_context.get_lines_limit() < 0) || (lines_count < m_context.get_lines_limit())))
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
            if (c == m_context.get_line_terminator())
            {
                ++lines_count;
                if (m_context.get_line_terminator() == '\0')
                {
                    c = '\n';
                }
            }

            std::cout << c;
        }
    }

    context m_context;
};

} // namespace oct_args_examples

int main(int argc, char* argv[])
{
    return oct_args_examples::head_app().run(argc, argv);
}
