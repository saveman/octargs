#include <octargs/octargs.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace oct::args;

    enum class order
    {
        ascending,
        descending
    };
    struct order_converter
    {
        order operator()(const std::string& value) const
        {
            if (value == "asc")
                return order::ascending;
            else if (value == "desc")
                return order::descending;
            throw conversion_error_ex<char>(value);
        }
    };

    parser arg_parser;
    arg_parser.add_valued({ "--order" })
        .set_min_count(1)
        .set_allowed_values({ "asc", "desc" })
        .set_default_value("asc")
        .set_type<order, order_converter>();
    arg_parser.add_positional("files").set_max_count_unlimited();

    //! [Snippet]
    try
    {
        auto results = arg_parser.parse(argc, argv);

        auto requested_order = results.get_first_value("--order");
        auto files = results.get_values("files");

        // TODO: application logic
    }
    catch (const parser_error_ex<char>& e)
    {
        switch (e.get_error_code())
        {
        case parser_error_code::CONVERSION_FAILED:
            std::cerr << "ERROR: Cannot convert value '" << e.get_value() << "' for argument '" << e.get_name() << "'"
                      << std::endl;
            break;
        case parser_error_code::TOO_MANY_OCCURRENCES:
            std::cerr << "ERROR: Argument '" << e.get_name() << "' provided too many times" << std::endl;
            break;

        case parser_error_code::SYNTAX_ERROR:
            std::cerr << "ERROR: Syntax error - cannot parse: '" << e.get_value() << "'" << std::endl;
            break;

            // TODO: other

        default:
            std::cerr << "ERROR: Cannot parse input arguments" << std::endl;
            break;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "FATAL error: " << e.what() << std::endl;
    }
    //! [Snippet]

    return 0;
}
