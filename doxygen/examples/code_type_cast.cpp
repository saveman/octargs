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

    try
    {
        auto results = arg_parser.parse(argc, argv);

        //! [Snippet]
        auto requested_order = results.get_first_value_as<order, order_converter>("--order");
        auto files = results.get_values_as<std::string>("files");
        //! [Snippet]

        // TODO: application logic
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred: " << e.what() << std::endl;
    }

    return 0;
}
