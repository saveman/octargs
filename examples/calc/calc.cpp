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

    template <typename data_T>
    static void execute_sum(std::ostream& os, const std::vector<data_T>& values, bool show_steps)
    {
        using data_type = data_T;

        if (show_steps)
        {
            data_type result = 0;

            os << "Init: " << result << std::endl;
            for (auto& v : values)
            {
                auto prev = result;
                result += v;

                os << "Step: " << prev << " + " << v << " = " << result << std::endl;
            }
            os << "Result: " << result << std::endl;
        }

        os << std::accumulate(values.begin(), values.end(), data_type(0)) << std::endl;
    }

    template <typename data_T>
    static void execute_mul(std::ostream& os, const std::vector<data_T>& values, bool show_steps)
    {
        using data_type = data_T;

        if (show_steps)
        {
            data_type result = 1;

            os << "Init: " << result << std::endl;
            for (auto& v : values)
            {
                auto prev = result;
                result *= v;

                os << "Step: " << prev << " x " << v << " = " << result << std::endl;
            }
            os << "Result: " << result << std::endl;
        }

        os << std::accumulate(values.begin(), values.end(), data_type(1), [](const data_type& v1, const data_type& v2) {
            return v1 * v2;
        }) << std::endl;
    }

    template <typename data_T>
    static void execute_min(std::ostream& os, const std::vector<data_T>& values, bool show_steps)
    {
        using data_type = data_T;

        if (show_steps)
        {
            data_type result = values[0];

            os << "Init (first): " << result << std::endl;
            for (auto& v : values)
            {
                auto prev = result;
                result = std::min(result, v);

                os << "Step: " << prev << " vs. " << v << " => " << result << std::endl;
            }
            os << "Result: " << result << std::endl;
        }

        os << *std::min_element(values.begin(), values.end()) << std::endl;
    }

    template <typename data_T>
    static void execute_max(std::ostream& os, const std::vector<data_T>& values, bool show_steps)
    {
        using data_type = data_T;

        if (show_steps)
        {
            data_type result = values[0];

            os << "Init (first): " << result << std::endl;
            for (auto& v : values)
            {
                auto prev = result;
                result = std::max(result, v);

                os << "Step: " << prev << " vs. " << v << " => " << result << std::endl;
            }
            os << "Result: " << result << std::endl;
        }

        os << *std::max_element(values.begin(), values.end()) << std::endl;
    }

    template <typename data_T>
    static void execute(std::ostream& os, const std::vector<data_T>& values, operation_code operation, bool show_steps)
    {
        switch (operation)
        {
        case operation_code::SUM:
            execute_sum(os, values, show_steps);
            break;

        case operation_code::MUL:
            execute_mul(os, values, show_steps);
            break;

        case operation_code::MIN:
            execute_min(os, values, show_steps);
            break;

        case operation_code::MAX:
            execute_max(os, values, show_steps);
            break;

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

            arg_parser.set_usage_oneliner("Simple calculator application");
            arg_parser.set_usage_header("Performs requested operation on given values.");

            arg_parser.add_exclusive({ "--help" }).set_description("display this help and exit");
            arg_parser.add_valued({ "-o", "--oper", "--operation" })
                .set_description("operation to perform")
                .set_min_count(1)
                .set_allowed_values({ "sum", "mul", "min", "max" })
                .set_default_value("sum");
            arg_parser.add_valued({ "-t", "--type" })
                .set_description("operand type")
                .set_min_count(1)
                .set_allowed_values({ "int", "float", "double" })
                .set_default_value("int");
            arg_parser.add_positional("OPERANDS")
                .set_description("values on which operations will be performed")
                .set_min_count(1)
                .set_max_count_unlimited();

            auto output_group = arg_parser.add_group("Output arguments");
            output_group.add_switch({ "-s", "--steps" }).set_description("show output of steps");

            auto results = arg_parser.parse(argc, argv);

            if (results.has_value("--help"))
            {
                std::cout << arg_parser.get_usage() << std::endl;
                return EXIT_SUCCESS;
            }

            auto operation = results.get_first_value_as<operation_code, operation_code_converter>("-o");
            auto data_type = results.get_first_value_as<data_type_code, data_type_code_converter>("-t");
            auto show_steps = results.has_value("-s");

            switch (data_type)
            {
            case data_type_code::INT:
            {
                execute(std::cout, results.get_values_as<int>("OPERANDS"), operation, show_steps);
                break;
            }
            case data_type_code::FLOAT:
            {
                execute(std::cout, results.get_values_as<float>("OPERANDS"), operation, show_steps);
                break;
            }
            case data_type_code::DOUBLE:
            {
                execute(std::cout, results.get_values_as<double>("OPERANDS"), operation, show_steps);
                break;
            }
            default:
                throw std::logic_error("unsupported data type");
            }
        }
        catch (const oct::args::parser_error_ex<char>& exc)
        {
            std::cerr << "Argument parsing error near: " << exc.get_name() << " " << exc.get_value() << std::endl;
            std::cerr << "Run " << argv[0] << " --help to see usage information" << std::endl;
            return EXIT_FAILURE;
        }
        catch (const oct::args::conversion_error_ex<char>& exc)
        {
            std::cerr << "Incorrect value: " << exc.get_value() << std::endl;
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
