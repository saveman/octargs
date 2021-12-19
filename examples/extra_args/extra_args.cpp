#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "octargs/octargs.hpp"

namespace oct_args_examples
{

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

struct extra_args_app_settings
{
    explicit extra_args_app_settings()
        : m_help_requested(false)
        , m_hex_input(false)
    {
        // noop
    }

    bool m_help_requested;
    bool m_hex_input;
};

class extra_args_app_engine
{
public:
    explicit extra_args_app_engine(const extra_args_app_settings& settings)
        : m_settings(settings)
    {
        // noop
    }

    void execute(const std::vector<std::string>& args)
    {
        int sum = 0;
        int base = m_settings.m_hex_input ? 16 : 10;

        for (auto& arg : args)
        {
            sum += std::stoi(arg, nullptr, base);
        }

        std::cout << sum << std::endl;
    }

private:
    extra_args_app_settings m_settings;
};

class extra_args_app
{
public:
    extra_args_app(int argc, char* argv[])
        : m_input_args(argc, argv)
    {
        // noop
    }

    int run()
    {
        oct::args::storing_parser<extra_args_app_settings> arg_parser;

        try
        {
            arg_parser
                .set_usage_oneliner("Example - parse some args using parser, the rest handle internally by the app.")
                .set_usage_header("This is pure example, not a working, functional app. It sums the given values.");
            arg_parser.add_exclusive({ "--help" })
                .set_description("show help information")
                .set_type_and_storage(&extra_args_app_settings::m_help_requested);
            arg_parser.add_switch({ "-h", "--hex" })
                .set_description("treat input value as hex numbers")
                .set_type_and_storage(&extra_args_app_settings::m_hex_input);
            arg_parser.set_allow_unconsummed(true);

            extra_args_app_settings settings;

            auto results = arg_parser.parse(m_input_args, settings);

            if (settings.m_help_requested)
            {
                std::cout << arg_parser.get_usage() << std::endl;
                return EXIT_SUCCESS;
            }
            extra_args_app_engine(settings).execute(results.get_unconsummed_args());
        }
        catch (const oct::args::parser_error_ex<char>& exc)
        {
            std::cerr << "Argument parsing error near: " << exc.get_name() << " " << exc.get_value() << std::endl;
            std::cerr << "Run " << m_input_args.get_app_name() << " --help to see usage information" << std::endl;
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
    try
    {
        return oct_args_examples::extra_args_app(argc, argv).run();
    }
    catch (const std::exception& exc)
    {
        std::cerr << "FATAL ERROR: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "FATAL ERROR: unknown error occurred" << std::endl;
        return EXIT_FAILURE;
    }
}
