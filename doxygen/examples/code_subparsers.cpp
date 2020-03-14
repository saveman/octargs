#include <octargs/octargs.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace oct::args;

    parser arg_parser;
    arg_parser.add_switch({ "--verbose" }); // common argument

    //! [Snippet Add]
    auto& subparsers = arg_parser.add_subparsers("command");
    //! [Snippet Add]

    //! [Snippet Configure]
    auto add_parser = subparsers.add_parser("add");
    add_parser.add_switch({ "--recursive" });
    add_parser.add_positional("files").set_max_count_unlimited();

    auto commit_parser = subparsers.add_parser("commit");
    commit_parser.add_valued({ "--message" });
    //! [Snippet Configure]

    auto results = arg_parser.parse(argc, argv);

    std::cout << "Command requested: " << results.get_first_value("command") << std::endl;

    // TODO: application logic

    return 0;
}
