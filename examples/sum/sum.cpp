#include <clocale>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>

#include "octargs/octargs.hpp"

namespace oct_args_examples
{

enum class locale_key
{
    USAGE_ONELINER,
    USAGE_HEADER,
    TYPE_ARG_DESCRIPTION,
    HELP_ARG_DESCRIPTION,
    OPERANDS_ARG_DESCRIPTION,
    OPERANDS_VALUE_NAME,
    STEPS_ARG_DESCRIPTION,
    NAMED_ARGS_GROUP_NAME,
    POSITIONAL_ARGS_GROUP_NAME,
    OUTPUT_ARGS_GROUP_NAME,
    SWITCH_ENABLED_LITERAL,
    TRUE_LITERAL,
    FALSE_LITERAL,
    USAGE_LEAD,
    USAGE_DECORATOR_REQUIRED,
    USAGE_DECORATOR_UNLIMITED,
    USAGE_DECORATOR_DEFAULT,
    USAGE_DECORATOR_ALLOWED,
    USAGE_HEADER_OPTIONS_NAME,
};

using locale_map_type = std::map<locale_key, std::string>;

const locale_map_type LOCALE_DEFAULT {
    { locale_key::USAGE_ONELINER, "Simple sum calculator" },
    { locale_key::USAGE_HEADER, "Calculates sum of given values." },
    { locale_key::TYPE_ARG_DESCRIPTION, "operand type" },
    { locale_key::HELP_ARG_DESCRIPTION, "display this help and exit" },
    { locale_key::OPERANDS_ARG_DESCRIPTION, "values on which operations will be performed" },
    { locale_key::OPERANDS_VALUE_NAME, "OPERANDS" },
    { locale_key::STEPS_ARG_DESCRIPTION, "show output of steps" },
    { locale_key::OUTPUT_ARGS_GROUP_NAME, "Output arguments" },
};

const locale_map_type LOCALE_PL {
    { locale_key::USAGE_ONELINER, "Prosty kalkulator sum" },
    { locale_key::USAGE_HEADER, "Oblicza sumę podanych wartości." },
    { locale_key::TYPE_ARG_DESCRIPTION, "typ wartości" },
    { locale_key::HELP_ARG_DESCRIPTION, "pokaż informacje o pomocy i zakończ program" },
    { locale_key::OPERANDS_ARG_DESCRIPTION, "wartości na których obliczenia zostaną przeprowadzone" },
    { locale_key::OPERANDS_VALUE_NAME, "WARTOŚCI" },
    { locale_key::STEPS_ARG_DESCRIPTION, "pokaż wyniki poszczególnych kroków" },
    { locale_key::OUTPUT_ARGS_GROUP_NAME, "Argumenty kontrolujące dane wyjściowe" },
    { locale_key::NAMED_ARGS_GROUP_NAME, "Argumenty opcjonalne" },
    { locale_key::POSITIONAL_ARGS_GROUP_NAME, "Argumenty pozycyjne" },
    { locale_key::SWITCH_ENABLED_LITERAL, "tak" },
    { locale_key::TRUE_LITERAL, "tak" },
    { locale_key::FALSE_LITERAL, "nie" },
    { locale_key::USAGE_LEAD, "Użycie" },
    { locale_key::USAGE_DECORATOR_REQUIRED, "wymagany" },
    { locale_key::USAGE_DECORATOR_UNLIMITED, "nielimitowany" },
    { locale_key::USAGE_DECORATOR_DEFAULT, "domyślnie" },
    { locale_key::USAGE_DECORATOR_ALLOWED, "dozwolone" },
    { locale_key::USAGE_HEADER_OPTIONS_NAME, "OPCJE" },
};

const locale_map_type& get_locale_map()
{
    setlocale(LC_ALL, "");

    auto current_c_locale = setlocale(LC_ALL, NULL);
    std::string current_locale = current_c_locale ? current_c_locale : "";

    if (current_locale.find("pl_PL") != std::string::npos)
    {
        return LOCALE_PL;
    }
    return LOCALE_DEFAULT;
}

class sum_app
{
    using parser_type = oct::args::parser;

    class dictionary : public oct::args::custom_dictionary<parser_type::char_type>
    {
    public:
        using base_type = oct::args::custom_dictionary<parser_type::char_type>;

        dictionary(const locale_map_type& locale)
            : base_type(base_type::WITH_DEFAULTS)
        {
            if (locale.count(locale_key::SWITCH_ENABLED_LITERAL))
            {
                set_switch_enabled_literal(locale.at(locale_key::SWITCH_ENABLED_LITERAL));
            }
            if (locale.count(locale_key::TRUE_LITERAL))
            {
                add_true_literal(locale.at(locale_key::TRUE_LITERAL));
            }
            if (locale.count(locale_key::FALSE_LITERAL))
            {
                add_false_literal(locale.at(locale_key::FALSE_LITERAL));
            }
            if (locale.count(locale_key::USAGE_DECORATOR_REQUIRED))
            {
                set_usage_literal(
                    dictionary::usage_literal::DECORATOR_REQUIRED, locale.at(locale_key::USAGE_DECORATOR_REQUIRED));
            }
            if (locale.count(locale_key::USAGE_DECORATOR_UNLIMITED))
            {
                set_usage_literal(dictionary::usage_literal::DECORATOR_MAX_COUNT_UNLIMITED,
                    locale.at(locale_key::USAGE_DECORATOR_UNLIMITED));
            }
            if (locale.count(locale_key::USAGE_DECORATOR_DEFAULT))
            {
                set_usage_literal(
                    dictionary::usage_literal::DECORATOR_DEFAULT, locale.at(locale_key::USAGE_DECORATOR_DEFAULT));
            }
            if (locale.count(locale_key::USAGE_DECORATOR_ALLOWED))
            {
                set_usage_literal(
                    dictionary::usage_literal::DECORATOR_ALLOWED, locale.at(locale_key::USAGE_DECORATOR_ALLOWED));
            }
            if (locale.count(locale_key::NAMED_ARGS_GROUP_NAME))
            {
                set_usage_literal(dictionary::usage_literal::DEFAULT_NAMED_ARGUMENTS_GROUP_NAME,
                    locale.at(locale_key::NAMED_ARGS_GROUP_NAME));
            }
            if (locale.count(locale_key::POSITIONAL_ARGS_GROUP_NAME))
            {
                set_usage_literal(dictionary::usage_literal::DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME,
                    locale.at(locale_key::POSITIONAL_ARGS_GROUP_NAME));
            }
            if (locale.count(locale_key::USAGE_LEAD))
            {
                set_usage_literal(dictionary::usage_literal::LEAD, locale.at(locale_key::USAGE_LEAD));
            }
            if (locale.count(locale_key::USAGE_HEADER_OPTIONS_NAME))
            {
                set_usage_literal(
                    dictionary::usage_literal::HEADER_OPTIONS_NAME, locale.at(locale_key::USAGE_HEADER_OPTIONS_NAME));
            }
        }
    };

    enum class data_type_code
    {
        INT,
        FLOAT,
        DOUBLE
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

public:
    sum_app() = default;

    int run(int argc, char* argv[])
    {
        try
        {
            auto& locale = get_locale_map();
            auto dictionary = create_dictionary(locale);
            auto parser = create_parser(locale, dictionary);
            auto arguments = parser.parse(argc, argv);

            return execute(parser, arguments, *dictionary);
        }
        catch (const oct::args::parser_error_ex<parser_type::char_type>& exc)
        {
            std::cerr << "Argument parsing error near: " << exc.get_name() << " " << exc.get_value() << std::endl;
            std::cerr << "Run " << argv[0] << " --help to see usage information" << std::endl;
            return EXIT_FAILURE;
        }
        catch (const oct::args::conversion_error_ex<parser_type::char_type>& exc)
        {
            std::cerr << "Incorrect value: " << exc.get_value() << std::endl;
            return EXIT_FAILURE;
        }
        catch (const std::exception& exc)
        {
            std::cerr << "FATAL ERROR: " << exc.what() << std::endl;
            return EXIT_FAILURE;
        }
    }

private:
    int execute(const parser_type& parser, const parser_type::results_type& results, const dictionary& /*dictionary*/)
    {
        if (results.has_value("--help"))
        {
            std::cout << parser.get_usage() << std::endl;
            return EXIT_SUCCESS;
        }

        auto data_type = results.get_first_value_as<data_type_code, data_type_code_converter>("-t");
        auto show_steps = results.get_first_value_as<bool>("-s");

        switch (data_type)
        {
        case data_type_code::INT:
        {
            execute(std::cout, results.get_values_as<int>("OPERANDS"), show_steps);
            break;
        }
        case data_type_code::FLOAT:
        {
            execute(std::cout, results.get_values_as<float>("OPERANDS"), show_steps);
            break;
        }
        case data_type_code::DOUBLE:
        {
            execute(std::cout, results.get_values_as<double>("OPERANDS"), show_steps);
            break;
        }
        default:
            throw std::logic_error("unsupported data type");
        }

        return EXIT_SUCCESS;
    }

    static std::shared_ptr<dictionary> create_dictionary(const locale_map_type& locale)
    {
        return std::make_shared<dictionary>(locale);
    }

    static parser_type create_parser(
        const locale_map_type& locale, const parser_type::const_dictionary_ptr_type& dictionary)
    {
        parser_type parser(dictionary);

        parser.set_usage_oneliner(locale.at(locale_key::USAGE_ONELINER));
        parser.set_usage_header(locale.at(locale_key::USAGE_HEADER));

        parser.add_exclusive({ "--help" }).set_description(locale.at(locale_key::HELP_ARG_DESCRIPTION));
        parser.add_valued({ "-t", "--type" })
            .set_description(locale.at(locale_key::TYPE_ARG_DESCRIPTION))
            .set_min_count(1)
            .set_allowed_values({ "int", "float", "double" })
            .set_default_value("int");
        parser.add_positional("OPERANDS")
            .set_value_name(locale.at(locale_key::OPERANDS_VALUE_NAME))
            .set_description(locale.at(locale_key::OPERANDS_ARG_DESCRIPTION))
            .set_min_count(1)
            .set_max_count_unlimited();

        auto output_group = parser.add_group(locale.at(locale_key::OUTPUT_ARGS_GROUP_NAME));
        output_group.add_valued({ "-s", "--steps" })
            .set_description(locale.at(locale_key::STEPS_ARG_DESCRIPTION))
            .set_value_name("STEPS")
            .set_default_value(*dictionary->get_false_literals().rbegin());

        return parser;
    }

    template <typename data_T>
    static void execute(std::ostream& os, const std::vector<data_T>& values, bool show_steps)
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
};

} // namespace oct_args_examples

int main(int argc, char* argv[])
{
    return oct_args_examples::sum_app().run(argc, argv);
}
