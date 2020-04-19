#include <octargs/octargs.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace oct::args;

    parser arg_parser;

    //! [Snippet Parser Info]
    arg_parser.set_usage_oneliner("Get file from HTTP server");
    arg_parser.set_usage_header("Downloads files from the specified host using\n"
                                "the HTTP protocol.");
    arg_parser.set_usage_header("More information could be found on http://example.com/");
    //! [Snippet Parser Info]

    arg_parser.add_switch({ "--verbose" }).set_description("print verbose debug messages");
    arg_parser.add_positional("URLs").set_description("URLs of files to download");

    //! [Snippet Switch]
    arg_parser.add_exclusive({ "--help", "-h" }).set_description("print usage information");
    //! [Snippet Switch]

    //! [Snippet Group]
    auto protocol_group = arg_parser.add_group("Protocol arguments:");
    protocol_group.add_switch({ "-0", "--use-http-1.0" }).set_description("use HTTP/1.0 instead of default HTTP/1.1");
    //! [Snippet Argument Info]
    protocol_group.add_valued({ "-a", "--user-agent" })
        .set_value_name("agent")
        .set_description("user agent sent in the request\nby default it is empty");
    //! [Snippet Argument Info]
    //! [Snippet Group]

    auto results = arg_parser.parse(argc, argv);

    //! [Snippet Print]
    if (results.has_value("-h"))
    {
        std::cout << arg_parser.get_usage() << std::endl;
        return 0;
    }
    //! [Snippet Print]

    // TODO application logic

    return 0;
}
