#include <octargs/octargs.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace oct::args;

    parser arg_parser;
    //! [Snippet]
    arg_parser.add_switch("--verbose");
    arg_parser.add_switch("--dry-run");
    //! [Snippet]

    auto results = arg_parser.parse(argc, argv);

    std::cout << "Verbose?   " << (results.has_value("--verbose") ? "yes" : "no") << std::endl;
    std::cout << "Test mode? " << (results.has_value("--dry-run") ? "yes" : "no") << std::endl;

    return 0;
}
