#ifndef OCTARGS_PARSER_ENGINE_HPP_
#define OCTARGS_PARSER_ENGINE_HPP_

#include <memory>
#include <string>

#include "../argument_table.hpp"
#include "../dictionary.hpp"
#include "../exception.hpp"
#include "../parser_error.hpp"
#include "../results.hpp"

#include "argument.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T, typename values_storage_T>
class basic_parser_engine
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;

    using argument_table_type = basic_argument_table<char_type>;
    using results_type = basic_results<char_type>;

    using storage_helper_type = storage_handler_helper<char_type, values_storage_type>;

    using parser_data_type = basic_parser_data<char_type, values_storage_type>;
    using parser_data_ptr_type = std::shared_ptr<parser_data_type>;

    basic_parser_engine(const argument_table_type& arg_table, storage_helper_type& storage_helper,
        const parser_data_ptr_type& root_parser_data_ptr)
        : m_arg_table(arg_table)
        , m_storage_helper(storage_helper)
        , m_root_parser_data_ptr(root_parser_data_ptr)
        , m_results_data_ptr(prepare_results_data(root_parser_data_ptr, m_arg_table.get_app_name()))
    {
        // noop
    }

    results_type parse()
    {
        argument_table_iterator exclusive_input_iterator(m_arg_table);

        if (!parse_exclusive_recursively(m_root_parser_data_ptr, exclusive_input_iterator))
        {
            argument_table_iterator regular_input_iterator(m_arg_table);

            parse_regular(m_root_parser_data_ptr, regular_input_iterator);
        }
        return results_type(m_root_parser_data_ptr->m_dictionary, m_results_data_ptr);
    }

private:
    using results_data_type = basic_results_data<char_type>;
    using results_data_ptr_type = std::shared_ptr<results_data_type>;

    using argument_type = basic_argument<char_type, values_storage_type>;
    using argument_table_iterator = basic_argument_table_iterator<char_type>;
    using const_argument_ptr_type = std::shared_ptr<const argument_type>;

    static void fill_results_data_names(const results_data_ptr_type& results_data_ptr,
        const parser_data_ptr_type& parser_data_ptr, const string_type& prefix)
    {
        for (auto& iter : parser_data_ptr->m_argument_repository->m_names_repository)
        {
            results_data_ptr->add_name(prefix + iter.first, iter.second);
        }

        if (!parser_data_ptr->m_argument_repository->m_subparsers_argument)
        {
            return;
        }

        for (auto& subparser_item : parser_data_ptr->get_subparsers())
        {
            auto new_prefix
                = prefix + subparser_item.first + parser_data_ptr->m_dictionary->get_subparser_separator_literal();

            fill_results_data_names(results_data_ptr, subparser_item.second, new_prefix);
        }
    }

    static results_data_ptr_type prepare_results_data(
        const parser_data_ptr_type& root_parser_data_ptr, const string_type& app_name)
    {
        auto results_data_ptr
            = std::make_shared<results_data_type>(root_parser_data_ptr->m_dictionary->is_case_sensitive());

        results_data_ptr->set_app_name(app_name);
        fill_results_data_names(results_data_ptr, root_parser_data_ptr, string_type());

        return results_data_ptr;
    }

    bool parse_exclusive_recursively(
        const parser_data_ptr_type& parser_data_ptr, argument_table_iterator& input_iterator) const
    {
        if (input_iterator.get_remaining_count() == 0)
        {
            // no arguments - exit
            return false;
        }
        else if (input_iterator.get_remaining_count() == 1)
        {
            auto& arg_name = input_iterator.take_next();

            auto arg_iter = parser_data_ptr->m_argument_repository->m_names_repository.find(arg_name);
            if (arg_iter == parser_data_ptr->m_argument_repository->m_names_repository.end())
            {
                // not an argument name
                return false;
            }

            auto& arg_object_ptr = arg_iter->second;

            if (!arg_object_ptr->is_exclusive())
            {
                // not exclusive
                return false;
            }

            auto& value_str = parser_data_ptr->m_dictionary->get_switch_enabled_literal();

            parse_argument_value(parser_data_ptr, arg_object_ptr, arg_name, value_str);

            return true;
        }
        else
        {
            if (!parser_data_ptr->m_argument_repository->m_subparsers_argument)
            {
                // no subparsers
                return false;
            }

            auto& arg_name = input_iterator.take_next();

            auto arg_iter = parser_data_ptr->m_argument_repository->m_names_repository.find(arg_name);
            if (arg_iter != parser_data_ptr->m_argument_repository->m_names_repository.end())
            {
                // argument name, subparser expected
                return false;
            }

            if (!parser_data_ptr->has_subparser(arg_name))
            {
                // not a subparser name
                return false;
            }

            return parse_exclusive_recursively(parser_data_ptr->get_subparser(arg_name), input_iterator);
        }
    }

    void parse_regular(const parser_data_ptr_type& parser_data_ptr, argument_table_iterator& input_iterator) const
    {
        parse_named_arguments(parser_data_ptr, input_iterator);
        if (parser_data_ptr->m_argument_repository->m_subparsers_argument)
        {
            /* all remaining arguments will go to subparser so process this parser defaults & requirements */
            parse_default_values(parser_data_ptr);
            check_values_count(parser_data_ptr);

            parse_subparsers_argument(parser_data_ptr, input_iterator);
        }
        else
        {
            parse_positional_arguments(parser_data_ptr, input_iterator);

            if (input_iterator.has_more())
            {
                throw parser_error_ex<char_type>(
                    parser_error_code::SYNTAX_ERROR, string_type(), input_iterator.peek_next());
            }

            parse_default_values(parser_data_ptr);
            check_values_count(parser_data_ptr);
        }
    }

    void parse_argument_value(const parser_data_ptr_type& parser_data_ptr, const const_argument_ptr_type& argument,
        const string_type& arg_name, const string_type& value_str) const
    {
        if (m_results_data_ptr->value_count(argument) >= argument->get_max_count())
        {
            throw parser_error_ex<char_type>(parser_error_code::TOO_MANY_OCCURRENCES, arg_name, value_str);
        }

        // check if the value is among the allowed
        auto& allowed_values = argument->get_allowed_values();
        if (!allowed_values.empty())
        {
            auto comparator = std::bind(string_equal<char_type>(parser_data_ptr->m_dictionary->is_case_sensitive()),
                value_str, std::placeholders::_1);

            if (std::find_if(allowed_values.begin(), allowed_values.end(), comparator) == allowed_values.end())
            {
                throw parser_error_ex<char_type>(parser_error_code::VALUE_NOT_ALLOWED, arg_name, value_str);
            }
        }

        // if there is a handler call it first, to make sure the value
        // is only stored if handler accepted it.
        auto handler = argument->get_handler();
        if (handler)
        {
            try
            {
                m_storage_helper.parse_with_handler(*handler, *parser_data_ptr->m_dictionary, value_str);
            }
            catch (const conversion_error&)
            {
                std::throw_with_nested(
                    parser_error_ex<char_type>(parser_error_code::CONVERSION_FAILED, arg_name, value_str));
            }
        }

        m_results_data_ptr->append_value(argument, value_str);
    }

    void parse_default_value(const parser_data_ptr_type& parser_data_ptr, const const_argument_ptr_type& argument) const
    {
        if (m_results_data_ptr->value_count(argument) > 0)
        {
            return;
        }

        auto& values = argument->get_default_values();
        if (values.empty())
        {
            return;
        }

        for (auto& value : values)
        {
            // TODO: should we throw logic_error instead of runtime_error if value is invalid?
            parse_argument_value(parser_data_ptr, argument, argument->get_first_name(), value);
        }
    }

    void parse_default_values(const parser_data_ptr_type& parser_data_ptr) const
    {
        for (auto& argument : parser_data_ptr->m_argument_repository->m_arguments)
        {
            parse_default_value(parser_data_ptr, argument);
        }
    }

    bool parse_named_argument(const parser_data_ptr_type& parser_data_ptr, argument_table_iterator& input_iterator,
        const string_type& arg_name) const
    {
        auto arg_iter = parser_data_ptr->m_argument_repository->m_names_repository.find(arg_name);
        if (arg_iter == parser_data_ptr->m_argument_repository->m_names_repository.end())
        {
            // not an argument name, goto positional arguments processing
            return false;
        }

        auto& arg_object_ptr = arg_iter->second;

        if (!arg_object_ptr->is_assignable_by_name() || arg_object_ptr->is_exclusive())
        {
            // not an named argument, goto positional arguments processing
            return false;
        }

        // argument found, so remove element from input
        input_iterator.take_next();

        string_type value_str;

        if (arg_object_ptr->is_accepting_separate_value())
        {
            if (!input_iterator.has_more())
            {
                throw parser_error_ex<char_type>(parser_error_code::VALUE_MISSING, arg_name, string_type());
            }

            value_str = input_iterator.take_next();
        }
        else
        {
            value_str = parser_data_ptr->m_dictionary->get_switch_enabled_literal();
        }

        parse_argument_value(parser_data_ptr, arg_object_ptr, arg_name, value_str);

        return true;
    }

    bool parse_named_argument(const parser_data_ptr_type& parser_data_ptr, argument_table_iterator& input_iterator,
        const string_type& arg_name, const string_type& arg_value) const
    {
        auto arg_iter = parser_data_ptr->m_argument_repository->m_names_repository.find(arg_name);
        if (arg_iter == parser_data_ptr->m_argument_repository->m_names_repository.end())
        {
            return false;
        }

        auto& arg_object_ptr = arg_iter->second;

        if (!arg_object_ptr->is_assignable_by_name() || arg_object_ptr->is_exclusive())
        {
            // not an named argument, goto positional arguments processing
            return false;
        }

        // argument found, so remove element from input
        input_iterator.take_next();

        if (!arg_object_ptr->is_accepting_immediate_value())
        {
            throw parser_error_ex<char_type>(parser_error_code::UNEXPECTED_VALUE, arg_name, arg_value);
        }

        parse_argument_value(parser_data_ptr, arg_object_ptr, arg_name, arg_value);
        return true;
    }

    bool parse_named_argument(
        const parser_data_ptr_type& parser_data_ptr, argument_table_iterator& input_iterator) const
    {
        auto& input_value = input_iterator.peek_next();

        auto& value_separator = parser_data_ptr->m_dictionary->get_value_separator_literal();

        auto value_separator_pos = input_value.find(value_separator);
        if (value_separator_pos == string_type::npos)
        {
            return parse_named_argument(parser_data_ptr, input_iterator, input_value);
        }
        else
        {
            auto name_str = input_value.substr(0, value_separator_pos);
            auto value_str = input_value.substr(value_separator_pos + value_separator.size());

            return parse_named_argument(parser_data_ptr, input_iterator, name_str, value_str);
        }
    }

    void parse_named_arguments(
        const parser_data_ptr_type& parser_data_ptr, argument_table_iterator& input_iterator) const
    {
        while (input_iterator.has_more())
        {
            if (!parse_named_argument(parser_data_ptr, input_iterator))
            {
                break;
            }
        }
    }

    void parse_subparsers_argument(
        const parser_data_ptr_type& parser_data_ptr, argument_table_iterator& input_iterator) const
    {
        auto& name = parser_data_ptr->m_argument_repository->m_subparsers_argument->get_first_name();

        if (!input_iterator.has_more())
        {
            throw parser_error_ex<char_type>(parser_error_code::SUBPARSER_NAME_MISSING, name, string_type());
        }

        auto& argument = parser_data_ptr->m_argument_repository->m_subparsers_argument;
        auto& value_str = input_iterator.take_next();

        if (!parser_data_ptr->has_subparser(value_str))
        {
            throw parser_error_ex<char_type>(parser_error_code::SUBPARSER_NOT_FOUND, name, string_type());
        }

        parse_argument_value(parser_data_ptr, argument, argument->get_first_name(), value_str);

        parse_regular(parser_data_ptr->get_subparser(value_str), input_iterator);
    }

    void parse_positional_arguments(
        const parser_data_ptr_type& parser_data_ptr, argument_table_iterator& input_iterator) const
    {
        for (auto& argument : parser_data_ptr->m_argument_repository->m_arguments)
        {
            if (argument->is_assignable_by_name())
            {
                continue;
            }

            while ((m_results_data_ptr->value_count(argument) < argument->get_max_count()) && input_iterator.has_more())
            {
                const auto& value_str = input_iterator.take_next();

                parse_argument_value(parser_data_ptr, argument, argument->get_first_name(), value_str);
            }
        }
    }

    void check_values_count(const parser_data_ptr_type& parser_data_ptr) const
    {
        for (auto& argument : parser_data_ptr->m_argument_repository->m_arguments)
        {
            if (m_results_data_ptr->value_count(argument) < argument->get_min_count())
            {
                throw parser_error_ex<char_type>(
                    parser_error_code::REQUIRED_ARGUMENT_MISSING, argument->get_first_name(), string_type());
            }
        }
    }

    const argument_table_type& m_arg_table;
    storage_helper_type& m_storage_helper;
    const parser_data_ptr_type& m_root_parser_data_ptr;

    results_data_ptr_type m_results_data_ptr;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_ENGINE_HPP_
