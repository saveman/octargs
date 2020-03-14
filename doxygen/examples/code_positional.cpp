#include <octargs/octargs.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace oct::args;

    parser arg_parser;
    //! [Snippet]
    arg_parser.add_positional("pattern").set_max_count(1);
    arg_parser.add_positional("files").set_max_count_unlimited();
    //! [Snippet]

    auto results = arg_parser.parse(argc, argv);

    if (results.get_count("pattern") == 0)
    {
        std::cout << "No pattern specified" << std::endl;
        return 0;
    }

    auto pattern = results.get_first_value("pattern");
    auto files = results.get_values("files");

    // TODO: search pattern in files

    return 0;
}
