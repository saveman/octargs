#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "octargs/octargs.hpp"

#include "protofile.hpp"
#include "protohttp.hpp"
#include "settings.hpp"

namespace oct_args_examples
{

class getfile_app
{
public:
    getfile_app(int argc, char* argv[])
        : m_input_args(argc, argv)
        , m_parser()
    {
        build_parser();
    }

    int run()
    {
        try
        {
            app_settings settings;

            m_parser.parse(m_input_args, settings);

            if (settings.m_help_requested)
            {
                std::cout << m_parser.get_usage() << std::endl;
                return EXIT_SUCCESS;
            }
            if (settings.m_protocol == "file")
            {
                file_get(settings.m_common, settings.m_file);
            }
            else if (settings.m_protocol == "http")
            {
                http_get(settings.m_common, settings.m_http);
            }
        }
        catch (const oct::args::parser_error_ex<char>& exc)
        {
            std::cerr << "Argument parsing error near: " << exc.get_name() << " " << exc.get_value() << std::endl;
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
    using parser_type = oct::args::storing_parser<app_settings>;

    int file_get(const common_settings& common_settings, const file_settings& file_settings)
    {
        if (file_settings.m_help_requested)
        {
            std::cout << m_file_parser.get_usage() << std::endl;
            return EXIT_SUCCESS;
        }

        file_getter getter(common_settings, file_settings);
        return getter.execute();
    }

    int http_get(const common_settings& common_settings, const http_settings& http_settings)
    {
        if (http_settings.m_help_requested)
        {
            std::cout << m_http_parser.get_usage() << std::endl;
            return EXIT_SUCCESS;
        }

        http_getter getter(common_settings, http_settings);
        return getter.execute();
    }

    void build_parser()
    {
        m_parser.set_usage_oneliner("Read file using different protocols");
        m_parser.set_usage_footer("Use <PROTOCOL> --help to get information about protocol options");

        m_parser.add_exclusive({ "--help" })
            .set_description("shows usage information")
            .set_type<bool>()
            .set_store_function([](app_settings& settings, bool value) { settings.m_help_requested = value; });
        m_parser.add_switch({ "-v", "--verbose" })
            .set_description("print verbose information")
            .set_type<bool>()
            .set_store_function([](app_settings& settings, bool value) { settings.m_common.m_verbose = value; });

        auto& subparsers = m_parser.add_subparsers("PROTOCOL").set_description("Protocol to use to get the file");

        subparsers.set_type<std::string>().set_store_function(
            [](app_settings& settings, const std::string& value) { settings.m_protocol = value; });

        m_file_parser = subparsers.add_parser("file");
        m_file_parser.set_usage_oneliner("Read local file");
        m_file_parser.add_exclusive({ "--help" })
            .set_description("shows usage information")
            .set_type<bool>()
            .set_store_function([](app_settings& settings, bool value) { settings.m_file.m_help_requested = value; });
        m_file_parser.add_valued({ "-p", "--path" })
            .set_description("path to file to get")
            .set_min_count(1)
            .set_type<std::string>()
            .set_store_function(
                [](app_settings& settings, const std::string& value) { settings.m_file.m_path = value; });

        m_http_parser = subparsers.add_parser("http");
        m_http_parser.set_usage_oneliner("Read file from HTTP server");
        m_http_parser.add_exclusive({ "--help" })
            .set_description("shows usage information")
            .set_type<bool>()
            .set_store_function([](app_settings& settings, bool value) { settings.m_http.m_help_requested = value; });
        m_http_parser.add_valued({ "-h", "--host" })
            .set_description("address of host where file is stored")
            .set_min_count(1)
            .set_type<std::string>()
            .set_store_function(
                [](app_settings& settings, const std::string& value) { settings.m_http.m_host = value; });
        m_http_parser.add_valued({ "-t", "--port" })
            .set_description("port of host where file is stored")
            .set_default_value("80")
            .set_type<std::uint16_t>()
            .set_store_function(
                [](app_settings& settings, const std::uint16_t& value) { settings.m_http.m_port = value; });
        m_http_parser.add_valued({ "-p", "--path" })
            .set_description("path to file to get")
            .set_min_count(1)
            .set_type<std::string>()
            .set_store_function(
                [](app_settings& settings, const std::string& value) { settings.m_http.m_path = value; });
    }

    oct::args::argument_table m_input_args;
    parser_type m_parser;
    parser_type m_file_parser;
    parser_type m_http_parser;
};

} // namespace oct_args_examples

int main(int argc, char* argv[])
{
    return oct_args_examples::getfile_app(argc, argv).run();
}
