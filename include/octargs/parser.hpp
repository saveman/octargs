#ifndef OCTARGS_PARSER_HPP_
#define OCTARGS_PARSER_HPP_

#include <memory>
#include <string>
#include <vector>

#include "argument.hpp"
#include "argument_group.hpp"
#include "argument_table.hpp"
#include "exception.hpp"
#include "exclusive_argument.hpp"
#include "parser_dictionary.hpp"
#include "positional_argument.hpp"
#include "results.hpp"
#include "subparser_argument.hpp"
#include "switch_argument.hpp"
#include "usage.hpp"
#include "valued_argument.hpp"

namespace oct
{
namespace args
{

enum class parser_error_code
{
    CONVERSION_FAILED,
    TOO_MANY_OCCURRENCES,
    SYNTAX_ERROR,
    VALUE_MISSING,
    UNEXPECTED_VALUE,
    REQUIRED_ARGUMENT_MISSING,
    SUBPARSER_NAME_MISSING,
    SUBPARSER_NOT_FOUND,
    VALUE_NOT_ALLOWED,
};

class parser_error : public std::runtime_error
{
public:
    explicit parser_error(parser_error_code code)
        : std::runtime_error("parsing error occurred")
        , m_error_code(code)
    {
        // noop
    }

    parser_error_code error_code() const
    {
        return m_error_code;
    }

private:
    parser_error_code m_error_code;
};

template <class char_T>
class parser_error_ex : public parser_error
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    explicit parser_error_ex(parser_error_code code, const string_type& name, const string_type& value)
        : parser_error(code)
        , m_name(name)
        , m_value(value)
    {
        // noop
    }

    const string_type& name() const
    {
        return m_name;
    }

    const string_type& value() const
    {
        return m_value;
    }

private:
    basic_shared_string<char_type> m_name;
    basic_shared_string<char_type> m_value;
};

template <typename char_T, typename values_storage_T = internal::null_values_storage>
class basic_parser
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using dictionary_type = parser_dictionary<char_type>;

    using argument_type = basic_argument<char_type, values_storage_type>;
    using switch_argument_type = basic_switch_argument<char_type, values_storage_type>;
    using valued_argument_type = basic_valued_argument<char_type, values_storage_type>;
    using positional_argument_type = basic_positional_argument<char_type, values_storage_type>;
    using exclusive_argument_type = basic_exclusive_argument<char_type, values_storage_type>;
    using subparser_argument_type = basic_subparser_argument<char_type, values_storage_type>;
    using argument_group_type = basic_argument_group<char_type, values_storage_type>;

    using argument_table_type = basic_argument_table<char_type>;
    using results_type = basic_results<char_type>;

    using parser_usage_type = basic_parser_usage<char_type, values_storage_type>;

    basic_parser()
        : m_data_ptr(std::make_shared<parser_data_type>())
    {
        // noop
    }

    parser_usage_type usage() const
    {
        return parser_usage_type(m_data_ptr);
    }

    basic_parser& set_usage_oneliner(const string_type& text)
    {
        m_data_ptr->m_usage_oneliner = text;
        return *this;
    }

    basic_parser& set_usage_header(const string_type& text)
    {
        m_data_ptr->m_usage_header = text;
        return *this;
    }

    basic_parser& set_usage_footer(const string_type& text)
    {
        m_data_ptr->m_usage_footer = text;
        return *this;
    }

    argument_group_type& add_group(const std::string& name)
    {
        return m_data_ptr->add_group(name);
    }

    exclusive_argument_type& add_exclusive(const string_vector_type& names)
    {
        return m_data_ptr->m_default_argument_group.add_exclusive(names);
    }

    switch_argument_type& add_switch(const string_vector_type& names)
    {
        return m_data_ptr->m_default_argument_group.add_switch(names);
    }

    valued_argument_type& add_valued(const string_vector_type& names)
    {
        return m_data_ptr->m_default_argument_group.add_valued(names);
    }

    positional_argument_type& add_positional(const string_type& name)
    {
        return m_data_ptr->m_default_argument_group.add_positional(name);
    }

    subparser_argument_type& add_subparsers(const string_type& name)
    {
        return *m_data_ptr->m_argument_repository.add_subparsers(name);
    }

    results_type parse(int argc, char_type* argv[], values_storage_type& values_storage = get_null_storage()) const
    {
        return parse_internal(argument_table_type(argc, argv), values_storage);
    }

    results_type parse(
        int argc, const char_type* argv[], values_storage_type& values_storage = get_null_storage()) const
    {
        return parse_internal(argument_table_type(argc, argv), values_storage);
    }

    results_type parse(
        const argument_table_type& arg_table, values_storage_type& values_storage = get_null_storage()) const
    {
        return parse_internal(arg_table, values_storage);
    }

private:
    using argument_table_iterator = basic_argument_table_iterator<char_type>;
    using argument_ptr_type = std::shared_ptr<argument_type>;
    using const_argument_ptr_type = std::shared_ptr<const argument_type>;
    using parser_data_type = internal::basic_parser_data<char_type, values_storage_type>;
    using parser_data_ptr_type = std::shared_ptr<parser_data_type>;
    using results_data_type = internal::basic_results_data<char_type>;
    using results_data_ptr_type = std::shared_ptr<results_data_type>;

    void fill_results_data_names(const string_type& prefix, const results_data_ptr_type& results_data_ptr) const
    {
        for (auto& iter : m_data_ptr->m_argument_repository.m_names_repository)
        {
            results_data_ptr->add_name(prefix + iter.first, iter.second);
        }

        if (!m_data_ptr->m_argument_repository.m_subparsers_argument)
        {
            return;
        }

        for (auto& subparser_item : m_data_ptr->m_argument_repository.m_subparsers_argument->get_parsers())
        {
            auto new_prefix = prefix + subparser_item.first + char_type(' ');

            subparser_item.second->fill_results_data_names(new_prefix, results_data_ptr);
        }
    }

    results_type parse_internal(const argument_table_type& arg_table, values_storage_type& values_storage) const
    {
        auto results_data_ptr = std::make_shared<results_data_type>();

        fill_results_data_names(string_type(), results_data_ptr);

        results_data_ptr->set_app_name(arg_table.get_app_name());

        argument_table_iterator input_iterator(arg_table);

        return parse_internal(results_data_ptr, values_storage, input_iterator);
    }

    results_type parse_internal(const results_data_ptr_type& results_data_ptr, values_storage_type& values_storage,
        argument_table_iterator& input_iterator) const
    {
        if (input_iterator.get_remaining_count() == 1)
        {
            if (parse_exclusive_argument(results_data_ptr, values_storage, input_iterator.peek_next()))
            {
                return results_type(results_data_ptr);
            }
        }

        parse_named_arguments(values_storage, results_data_ptr, input_iterator);
        if (m_data_ptr->m_argument_repository.m_subparsers_argument)
        {
            /* all remaining arguments will go to subparser so process this parser defaults & requirements */
            parse_default_values(results_data_ptr, values_storage);
            check_values_count(results_data_ptr);

            return parse_subparsers_argument(values_storage, results_data_ptr, input_iterator);
        }
        else
        {
            parse_positional_arguments(values_storage, results_data_ptr, input_iterator);

            if (input_iterator.has_more())
            {
                throw parser_error_ex<char_type>(
                    parser_error_code::SYNTAX_ERROR, string_type(), input_iterator.peek_next());
            }

            parse_default_values(results_data_ptr, values_storage);
            check_values_count(results_data_ptr);

            return results_type(results_data_ptr);
        }
    }

    static void parse_argument_value(const results_data_ptr_type& results_data_ptr, values_storage_type& values_storage,
        const const_argument_ptr_type& argument, const string_type& arg_name, const string_type& value_str)
    {
        auto count = results_data_ptr->value_count(argument);
        if (count >= argument->get_max_count())
        {
            throw parser_error_ex<char_type>(parser_error_code::TOO_MANY_OCCURRENCES, arg_name, value_str);
        }

        // check if the value is among the allowed
        auto& allowed_values = argument->get_allowed_values();
        if (!allowed_values.empty())
        {
            if (std::find(allowed_values.begin(), allowed_values.end(), value_str) == allowed_values.end())
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
                handler->parse(values_storage, value_str);
            }
            catch (const conversion_error&)
            {
                std::throw_with_nested(
                    parser_error_ex<char_type>(parser_error_code::CONVERSION_FAILED, arg_name, value_str));
            }
        }

        results_data_ptr->append_value(argument, value_str);
    }

    static void parse_default_value(const results_data_ptr_type& results_data_ptr, values_storage_type& values_storage,
        const const_argument_ptr_type& argument)
    {
        if (results_data_ptr->value_count(argument) > 0)
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
            parse_argument_value(results_data_ptr, values_storage, argument, argument->get_names()[0], value);
        }
    }

    void parse_default_values(const results_data_ptr_type& results_data_ptr, values_storage_type& values_storage) const
    {
        for (auto& argument : m_data_ptr->m_argument_repository.m_arguments)
        {
            parse_default_value(results_data_ptr, values_storage, argument);
        }
    }

    bool parse_exclusive_argument(const results_data_ptr_type& results_data_ptr, values_storage_type& values_storage,
        const string_type& arg_name) const
    {
        auto arg_iter = m_data_ptr->m_argument_repository.m_names_repository.find(arg_name);
        if (arg_iter == m_data_ptr->m_argument_repository.m_names_repository.end())
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

        auto& value_str = dictionary_type::get_switch_enabled_literal();

        parse_argument_value(results_data_ptr, values_storage, arg_object_ptr, arg_name, value_str);

        return true;
    }

    bool parse_named_argument(const results_data_ptr_type& results_data_ptr, values_storage_type& values_storage,
        argument_table_iterator& input_iterator, const string_type& arg_name) const
    {
        auto arg_iter = m_data_ptr->m_argument_repository.m_names_repository.find(arg_name);
        if (arg_iter == m_data_ptr->m_argument_repository.m_names_repository.end())
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
            value_str = dictionary_type::get_switch_enabled_literal();
        }

        parse_argument_value(results_data_ptr, values_storage, arg_object_ptr, arg_name, value_str);

        return true;
    }

    bool parse_named_argument(const results_data_ptr_type& results_data_ptr, values_storage_type& values_storage,
        argument_table_iterator& input_iterator, const string_type& arg_name, const string_type& arg_value) const
    {
        auto arg_iter = m_data_ptr->m_argument_repository.m_names_repository.find(arg_name);
        if (arg_iter == m_data_ptr->m_argument_repository.m_names_repository.end())
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

        parse_argument_value(results_data_ptr, values_storage, arg_object_ptr, arg_name, arg_value);
        return true;
    }

    bool parse_named_argument(const results_data_ptr_type& results_data_ptr, values_storage_type& values_storage,
        argument_table_iterator& input_iterator) const
    {
        auto& input_value = input_iterator.peek_next();

        auto equal_char_pos = input_value.find(dictionary_type::get_equal_literal());
        if (equal_char_pos == string_type::npos)
        {
            return parse_named_argument(results_data_ptr, values_storage, input_iterator, input_value);
        }
        else
        {
            auto name_str = input_value.substr(0, equal_char_pos);
            auto value_str = input_value.substr(equal_char_pos + 1);

            return parse_named_argument(results_data_ptr, values_storage, input_iterator, name_str, value_str);
        }
    }

    void parse_named_arguments(values_storage_type& values_storage, const results_data_ptr_type& results_data_ptr,
        argument_table_iterator& input_iterator) const
    {
        while (input_iterator.has_more())
        {
            if (!parse_named_argument(results_data_ptr, values_storage, input_iterator))
            {
                break;
            }
        }
    }

    results_type parse_subparsers_argument(values_storage_type& values_storage,
        const results_data_ptr_type& results_data_ptr, argument_table_iterator& input_iterator) const
    {
        auto& name = m_data_ptr->m_argument_repository.m_subparsers_argument->get_names()[0];

        if (!input_iterator.has_more())
        {
            throw parser_error_ex<char_type>(parser_error_code::SUBPARSER_NAME_MISSING, name, string_type());
        }

        auto& argument = m_data_ptr->m_argument_repository.m_subparsers_argument;
        auto& value_str = input_iterator.take_next();

        auto subparser = argument->get_parser(value_str);
        if (!subparser)
        {
            throw parser_error_ex<char_type>(parser_error_code::SUBPARSER_NOT_FOUND, name, string_type());
        }

        parse_argument_value(results_data_ptr, values_storage, argument, argument->get_names()[0], value_str);

        return subparser->parse_internal(results_data_ptr, values_storage, input_iterator);
    }

    void parse_positional_arguments(values_storage_type& values_storage, const results_data_ptr_type& results_data_ptr,
        argument_table_iterator& input_iterator) const
    {
        for (auto& argument : m_data_ptr->m_argument_repository.m_arguments)
        {
            if (argument->is_assignable_by_name())
            {
                continue;
            }

            while ((results_data_ptr->value_count(argument) < argument->get_max_count()) && input_iterator.has_more())
            {
                const auto& value_str = input_iterator.take_next();

                parse_argument_value(results_data_ptr, values_storage, argument, argument->get_names()[0], value_str);
            }
        }
    }

    void check_values_count(const results_data_ptr_type& results_data_ptr) const
    {
        for (auto& argument : m_data_ptr->m_argument_repository.m_arguments)
        {
            if (results_data_ptr->value_count(argument) < argument->get_min_count())
            {
                throw parser_error_ex<char_type>(
                    parser_error_code::REQUIRED_ARGUMENT_MISSING, argument->get_names()[0], string_type());
            }
        }
    }

    // TODO: try to refactor the way null storage is provided
    static values_storage_type& get_null_storage()
    {
        static values_storage_type null_storage;
        return null_storage;
    }

    parser_data_ptr_type m_data_ptr;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_HPP_
