#include <octargs/octargs.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace oct::args;

    parser arg_parser;

    //! [Snippet]
    arg_parser.add_switch({ "-v", "--verbose" }).set_min_count(0).set_max_count(3);
    arg_parser.add_valued({ "-n", "-l", "--lines" }).set_min_count(1);
    //! [Snippet]
    arg_parser.add_positional("files").set_max_count_unlimited();

    try
    {
        auto results = arg_parser.parse(argc, argv);

        auto verbosity_level = results.get_count("--verbose");
        auto lines = results.get_first_value("--lines");
        auto files = results.get_values("files");

        // TODO: application logic
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred: " << e.what() << std::endl;
    }

    return 0;
}
