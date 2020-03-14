#include <octargs/octargs.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace oct::args;

    parser arg_parser;
    //! [Snippet]
    arg_parser.add_valued({ "--order" })
        .set_min_count(1)
        .set_allowed_values({ "asc", "desc" })
        .set_default_value("asc");
    //! [Snippet]
    arg_parser.add_positional("files").set_max_count_unlimited();

    try
    {
        auto results = arg_parser.parse(argc, argv);

        auto order = results.get_first_value("--order");
        auto files = results.get_values("files");

        // TODO: application logic
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred: " << e.what() << std::endl;
    }

    return 0;
}
