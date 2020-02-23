#ifndef OCTARGS_PARSER_HPP_
#define OCTARGS_PARSER_HPP_

#include <memory>
#include <string>
#include <vector>

#include "argument.hpp"
#include "argument_table.hpp"
#include "exception.hpp"
#include "exclusive_argument.hpp"
#include "parser_dictionary.hpp"
#include "positional_argument.hpp"
#include "results.hpp"
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

    using dictionary_type = parser_dictionary<char_type>;
    using argument_type = basic_argument<char_type, values_storage_type>;
    using switch_argument_type = basic_switch_argument<char_type, values_storage_type>;
    using valued_argument_type = basic_valued_argument<char_type, values_storage_type>;
    using positional_argument_type = basic_positional_argument<char_type, values_storage_type>;
    using exclusive_argument_type = basic_exclusive_argument<char_type, values_storage_type>;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

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

    basic_parser& set_info_header(const string_type& text)
    {
        m_data_ptr->m_info_header = text;
        return *this;
    }

    basic_parser& set_info_footer(const string_type& text)
    {
        m_data_ptr->m_info_footer = text;
        return *this;
    }

    exclusive_argument_type& add_exclusive(const string_vector_type& names)
    {
        check_names(names);

        auto new_argument = std::make_shared<exclusive_argument_type>(names);

        add_to_names_repository(new_argument);
        m_data_ptr->m_exclusive_arguments.emplace_back(new_argument);

        return *new_argument;
    }

    switch_argument_type& add_switch(const string_vector_type& names)
    {
        check_names(names);

        auto new_argument = std::make_shared<switch_argument_type>(names);

        add_to_names_repository(new_argument);
        m_data_ptr->m_named_arguments.emplace_back(new_argument);

        return *new_argument;
    }

    valued_argument_type& add_valued(const string_vector_type& names)
    {
        check_names(names);

        auto new_argument = std::make_shared<valued_argument_type>(names);

        add_to_names_repository(new_argument);
        m_data_ptr->m_named_arguments.emplace_back(new_argument);

        return *new_argument;
    }

    positional_argument_type& add_positional(const string_type& name)
    {
        auto names = { name };

        check_names(names);

        auto new_argument = std::make_shared<positional_argument_type>(names);

        add_to_names_repository(new_argument);
        m_data_ptr->m_positional_arguments.emplace_back(new_argument);

        return *new_argument;
    }

    results_type parse(int argc, char_type* argv[], values_storage_type& values_storage = get_null_storage()) const
    {
        return parse(argument_table_type(argc, argv), values_storage);
    }

    results_type parse(
        int argc, const char_type* argv[], values_storage_type& values_storage = get_null_storage()) const
    {
        return parse(argument_table_type(argc, argv), values_storage);
    }

    results_type parse(
        const argument_table_type& arg_table, values_storage_type& values_storage = get_null_storage()) const
    {
        auto results_data_ptr = std::make_shared<results_data_type>(m_data_ptr);

        results_data_ptr->set_app_name(arg_table.get_app_name());

        if (arg_table.get_argument_count() == 1)
        {
            if (parse_exclusive(arg_table.get_argument(0), results_data_ptr, values_storage))
            {
                return results_type(results_data_ptr);
            }
        }

        argument_table_iterator input_iterator(arg_table);
        parse_named_arguments(input_iterator, values_storage, results_data_ptr);
        parse_positional_arguments(input_iterator, values_storage, results_data_ptr);
        if (input_iterator.has_more())
        {
            throw parser_error_ex<char_type>(
                parser_error_code::SYNTAX_ERROR, string_type(), input_iterator.peek_next());
        }

        parse_default_values(results_data_ptr, values_storage);
        check_values_count(results_data_ptr);

        return results_type(results_data_ptr);
    }

private:
    using argument_table_iterator = basic_argument_table_iterator<char_type>;
    using argument_ptr_type = std::shared_ptr<argument_type>;
    using parser_data_type = internal::basic_parser_data<char_type, values_storage_type>;
    using parser_data_ptr_type = std::shared_ptr<parser_data_type>;
    using results_data_type = internal::basic_results_data<char_type>;
    using results_data_ptr_type = std::shared_ptr<results_data_type>;

    static void parse_argument_value(const results_data_ptr_type& results_data_ptr, values_storage_type& values_storage,
        const argument_ptr_type& argument, const string_type& arg_name, const string_type& value_str)
    {
        auto count = results_data_ptr->value_count(argument);
        if (count >= argument->get_max_count())
        {
            throw parser_error_ex<char_type>(parser_error_code::TOO_MANY_OCCURRENCES, arg_name, value_str);
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
            catch (const conversion_error& exc)
            {
                std::throw_with_nested(
                    parser_error_ex<char_type>(parser_error_code::CONVERSION_FAILED, arg_name, value_str));
            }
        }

        results_data_ptr->append_value(argument, value_str);
    }

    static void parse_default_value(const results_data_ptr_type& results_data_ptr, values_storage_type& values_storage,
        const argument_ptr_type& argument)
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
        for (auto& argument : m_data_ptr->m_named_arguments)
        {
            parse_default_value(results_data_ptr, values_storage, argument);
        }
        for (auto& argument : m_data_ptr->m_positional_arguments)
        {
            parse_default_value(results_data_ptr, values_storage, argument);
        }
    }

    bool parse_exclusive(const string_type& arg_name, const results_data_ptr_type& results_data_ptr,
        values_storage_type& values_storage) const
    {
        auto arg_iter = m_data_ptr->m_names_repository.find(arg_name);
        if (arg_iter == m_data_ptr->m_names_repository.end())
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
        auto arg_iter = m_data_ptr->m_names_repository.find(arg_name);
        if (arg_iter == m_data_ptr->m_names_repository.end())
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
        auto arg_iter = m_data_ptr->m_names_repository.find(arg_name);
        if (arg_iter == m_data_ptr->m_names_repository.end())
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

    void parse_named_arguments(argument_table_iterator& input_iterator, values_storage_type& values_storage,
        const results_data_ptr_type& results_data_ptr) const
    {
        while (input_iterator.has_more())
        {
            if (!parse_named_argument(results_data_ptr, values_storage, input_iterator))
            {
                break;
            }
        }
    }

    void parse_positional_arguments(argument_table_iterator& input_iterator, values_storage_type& values_storage,
        const results_data_ptr_type& results_data_ptr) const
    {
        for (auto& argument : m_data_ptr->m_positional_arguments)
        {
            while ((results_data_ptr->value_count(argument) < argument->get_max_count()) && input_iterator.has_more())
            {
                const auto& value_str = input_iterator.take_next();

                parse_argument_value(results_data_ptr, values_storage, argument, argument->get_names()[0], value_str);
            }
        }
    }

    void check_values_count(const results_data_ptr_type& results_data_ptr) const
    {
        for (auto& argument : m_data_ptr->m_named_arguments)
        {
            if (results_data_ptr->value_count(argument) < argument->get_min_count())
            {
                throw parser_error_ex<char_type>(
                    parser_error_code::REQUIRED_ARGUMENT_MISSING, argument->get_names()[0], string_type());
            }
        }

        for (auto& argument : m_data_ptr->m_positional_arguments)
        {
            if (results_data_ptr->value_count(argument) < argument->get_min_count())
            {
                throw parser_error_ex<char_type>(
                    parser_error_code::REQUIRED_ARGUMENT_MISSING, argument->get_names()[0], string_type());
            }
        }
    }

    void check_names(const string_vector_type& names)
    {
        auto name_count = names.size();

        if (name_count < 1)
        {
            throw invalid_argument_name_ex<char_type>("no names given", string_type());
        }

        ensure_names_characters_valid(names);
        ensure_no_duplicated_names(names);
        ensure_names_not_registered(names);
    }

    void ensure_names_not_registered(const string_vector_type& names)
    {
        for (const auto& name : names)
        {
            if (m_data_ptr->m_names_repository.find(name) != m_data_ptr->m_names_repository.end())
            {
                throw invalid_argument_name_ex<char_type>("argument with given name already registered", name);
            }
        }
    }

    static void ensure_no_duplicated_names(const string_vector_type& names)
    {
        auto name_count = names.size();

        for (std::size_t i = 0; i < name_count; ++i)
        {
            for (std::size_t j = i + 1; j < name_count; ++j)
            {
                if (names[i] == names[j])
                {
                    throw invalid_argument_name_ex<char_type>("duplicated name", names[i]);
                }
            }
        }
    }

    void add_to_names_repository(const std::shared_ptr<argument_type>& argument)
    {
        for (const auto& name : argument->get_names())
        {
            m_data_ptr->m_names_repository.emplace(name, argument);
        }
    }

    static void ensure_names_characters_valid(const string_vector_type& names)
    {
        for (const auto& name : names)
        {
            ensure_name_characters_valid(name);
        }
    }

    static void ensure_name_characters_valid(const string_type& name)
    {
        if (name.empty())
        {
            throw invalid_argument_name_ex<char_type>("argument name must not be empty", name);
        }

        for (auto c : name)
        {
            if (internal::is_space(c))
            {
                throw invalid_argument_name_ex<char_type>("argument name must not contain whitespace characters", name);
            }
            if (c == dictionary_type::get_equal_literal())
            {
                throw invalid_argument_name_ex<char_type>("argument name must not contain equal characters", name);
            }
        }
    }

    // TODO: try to refactor the way null storage is provided
    static values_storage_type& get_null_storage()
    {
        static values_storage_type null_storage;
        return null_storage;
    }

    const parser_data_ptr_type m_data_ptr;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_HPP_
