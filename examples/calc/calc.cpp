#include <cstdint>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>

#include "octargs/octargs.hpp"

namespace oct_args_examples
{

class calc_app
{
    enum class operation_code
    {
        SUM,
        MUL,
        MIN,
        MAX
    };

    enum class data_type_code
    {
        INT,
        FLOAT,
        DOUBLE
    };

    class operation_code_converter
    {
    public:
        operation_code operator()(const std::string& value_str) const
        {
            if (value_str == "sum")
            {
                return operation_code::SUM;
            }
            else if (value_str == "mul")
            {
                return operation_code::MUL;
            }
            else if (value_str == "min")
            {
                return operation_code::MIN;
            }
            else if (value_str == "max")
            {
                return operation_code::MAX;
            }
            throw oct::args::conversion_error_ex<char>(value_str);
        }
    };

    class data_type_code_converter
    {
    public:
        data_type_code operator()(const std::string& value_str) const
        {
            if (value_str == "int")
            {
                return data_type_code::INT;
            }
            else if (value_str == "float")
            {
                return data_type_code::FLOAT;
            }
            else if (value_str == "double")
            {
                return data_type_code::DOUBLE;
            }
            throw oct::args::conversion_error_ex<char>(value_str);
        }
    };

    template <class data_T>
    static data_T execute(const std::vector<data_T>& values, operation_code operation)
    {
        using data_type = data_T;

        switch (operation)
        {
        case operation_code::SUM:
            return std::accumulate(values.begin(), values.end(), data_type(0));
        case operation_code::MUL:
            return std::accumulate(values.begin(), values.end(), data_type(1),
                [](const data_type& v1, const data_type& v2) { return v1 * v2; });
        case operation_code::MIN:
            return *std::min_element(values.begin(), values.end());
        case operation_code::MAX:
            return *std::max_element(values.begin(), values.end());
        default:
            throw std::logic_error("unsupported operation");
        }
    }

public:
    calc_app() = default;

    int run(int argc, char* argv[])
    {
        try
        {
            oct::args::parser arg_parser;

            arg_parser.set_info_header(
                "Simple calculator application\n\nPerforms requested operation on given values.");

            arg_parser.add_exclusive({ "--help" }).set_description("display this help and exit");
            // TODO: change when support for 'allowed values' is added
            arg_parser.add_valued({ "-o", "--oper", "--operation" })
                .set_description("operation to perform (sum, mul, min, max), default: sum")
                .set_min_count(1)
                .set_default_value("sum")
                .set_type<operation_code, operation_code_converter>();
            arg_parser.add_valued({ "-t", "--type" })
                .set_description("operand type (int, float, double), default: int")
                .set_min_count(1)
                .set_default_value("int");
            arg_parser.add_positional("OPERANDS")
                .set_description("values on which operations will be performed")
                .set_min_count(1)
                .set_max_count_unlimited();

            auto results = arg_parser.parse(argc, argv);

            if (results.has_value("--help"))
            {
                std::cout << arg_parser.usage() << std::endl;
                return EXIT_SUCCESS;
            }

            auto operation = results.as<operation_code, operation_code_converter>("-o");
            auto data_type = results.as<data_type_code, data_type_code_converter>("-t");

            switch (data_type)
            {
            case data_type_code::INT:
            {
                std::cout << execute(results.as_vector<int>("OPERANDS"), operation) << std::endl;
                break;
            }
            case data_type_code::FLOAT:
            {
                std::cout << execute(results.as_vector<float>("OPERANDS"), operation) << std::endl;
                break;
            }
            case data_type_code::DOUBLE:
            {
                std::cout << execute(results.as_vector<double>("OPERANDS"), operation) << std::endl;
                break;
            }
            default:
                throw std::logic_error("unsupported data type");
            }
        }
        catch (const oct::args::parser_error_ex<char>& exc)
        {
            std::cerr << "Argument parsing error near: " << exc.name() << " " << exc.value() << std::endl;
            std::cerr << "Run " << argv[0] << " --help to see usage information" << std::endl;
            return EXIT_FAILURE;
        }
        catch (const oct::args::conversion_error_ex<char>& exc)
        {
            std::cerr << "Incorrect value: " << exc.value() << std::endl;
            return EXIT_FAILURE;
        }
        catch (const std::exception& exc)
        {
            std::cerr << "FATAL ERROR: " << exc.what() << std::endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }
};

} // namespace oct_args_examples

int main(int argc, char* argv[])
{
    return oct_args_examples::calc_app().run(argc, argv);
}
