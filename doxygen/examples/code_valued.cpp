#include <octargs/octargs.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace oct::args;

    parser arg_parser;
    //! [Snippet]
    arg_parser.add_valued({ "--lines" });
    //! [Snippet]
    arg_parser.add_positional("files").set_max_count_unlimited();

    auto results = arg_parser.parse(argc, argv);

    std::string default_lines = "5";
    if (results.get_count("--lines") > 0)
    {
        default_lines = results.get_first_value("--lines");
    }

    // TODO

    return 0;
}
