#include <octargs/octargs.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace oct::args;

    //! [Snippet Type]
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
    //! [Snippet Type]

    parser arg_parser;
    //! [Snippet Configuration]
    arg_parser.add_valued({ "--order" })
        .set_min_count(1)
        .set_allowed_values({ "asc", "desc" })
        .set_default_value("asc")
        .set_type<order>()
        .set_convert_function(order_converter());
    //! [Snippet Configuration]
    arg_parser.add_positional("files").set_max_count_unlimited();

    try
    {
        auto results = arg_parser.parse(argc, argv);

        auto requested_order = results.get_first_value("--order");
        auto files = results.get_values("files");

        // TODO: application logic
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred: " << e.what() << std::endl;
    }

    return 0;
}
