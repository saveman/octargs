#include <octargs/octargs.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace oct::args;

    //! [Snippet]
    using dictionary_type = custom_dictionary<char>;
    auto dictionary = std::make_shared<dictionary_type>(dictionary_type::init_mode::WITH_DEFAULTS);
    dictionary->set_case_sensitive(false);
    dictionary->set_short_name_prefix("");
    dictionary->set_long_name_prefix("/");
    dictionary->set_value_separator_literal(":");
    parser arg_parser(dictionary);
    //! [Snippet]

    arg_parser.add_valued({ "/l", "/lines" });
    arg_parser.add_switch({ "/v", "/verbose" });

    auto results = arg_parser.parse(argc, argv);

    auto files = results.has_value("/verbose");

    // TODO: application logic

    return 0;
}
