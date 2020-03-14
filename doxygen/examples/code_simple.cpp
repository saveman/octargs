#include <octargs/octargs.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace oct::args;

    parser arg_parser;
    //! [Snippet]
    arg_parser.add_positional("files").set_max_count_unlimited();
    //! [Snippet]

    auto results = arg_parser.parse(argc, argv);

    auto files = results.get_values("files");

    for (auto& file : files)
    {
        std::cout << "File given: " << file << std::endl;
    }

    return 0;
}
