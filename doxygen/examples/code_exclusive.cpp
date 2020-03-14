#include <octargs/octargs.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace oct::args;

    parser arg_parser;
    arg_parser.add_switch({ "--verbose" });
    arg_parser.add_switch({ "--dry-run" });
    //! [Snippet]
    arg_parser.add_exclusive({ "--version" });
    //! [Snippet]

    auto results = arg_parser.parse(argc, argv);

    if (results.get_count("--version") > 0)
    {
        std::cout << "Example 1.0.0" << std::endl;
        return 0;
    }

    std::cout << "Verbose?   " << (results.get_count("--verbose") > 0 ? "yes" : "no") << std::endl;
    std::cout << "Test mode? " << (results.get_count("--dry-run") > 0 ? "yes" : "no") << std::endl;

    return 0;
}
