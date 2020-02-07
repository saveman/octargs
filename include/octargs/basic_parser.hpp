#ifndef OCTARGS_BASIC_PARSER_HPP_
#define OCTARGS_BASIC_PARSER_HPP_

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "basic_argument.hpp"
#include "basic_argument_table.hpp"
#include "basic_results.hpp"
#include "exception.hpp"
#include "internal/basic_parser_data.hpp"
#include "internal/basic_results_data.hpp"
#include "internal/char_utils.hpp"

namespace oct
{
namespace args
{
template <class TRAITS>
class basic_parser
{
public:
    using char_type = typename TRAITS::char_type;
    using string_type = typename TRAITS::string_type;
    using string_vector_type = typename TRAITS::string_vector_type;

    using argument_type = basic_argument<TRAITS>;
    using argument_table_type = basic_argument_table<TRAITS>;
    using results_type = basic_results<TRAITS>;
    using char_utils_type = internal::char_utils<char_type>;

    basic_parser()
        : m_data_ptr(std::make_shared<parser_data_type>())
    {
        // noop
    }

    void set_positional_arguments_enabled(bool enabled)
    {
        ensure_not_locked();
        m_data_ptr->m_positional_arguments_enabled = enabled;
    }

    void add_switch(const string_vector_type& names)
    {
        ensure_not_locked();
        check_names(names);

        auto new_argument = std::make_shared<argument_type>(names, false);

        m_data_ptr->m_arguments.emplace_back(new_argument);
        add_to_names_repository(new_argument);
    }

    void lock()
    {
        m_data_ptr->m_locked = true;
    }

    results_type parse(int argc, char_type* argv[])
    {
        return parse(argument_table_type(argc, argv));
    }

    results_type parse(int argc, const char_type* argv[])
    {
        return parse(argument_table_type(argc, argv));
    }

    results_type parse(const argument_table_type& arg_table)
    {
        lock();

        auto results_data_ptr = std::make_shared<results_data_type>(m_data_ptr);

        results_data_ptr->set_input(arg_table);
        results_data_ptr->set_app_name(arg_table.get_app_name());

        argument_table_iterator input_iterator(arg_table);

        // parse named arguments
        while (input_iterator.has_more())
        {
            if (!parse_named_argument(input_iterator, results_data_ptr))
            {
                break;
            }
        }

        // parse positional arguments
        while (input_iterator.has_more())
        {
            parse_positional_argument(input_iterator, results_data_ptr);
        }

        return results_type(results_data_ptr);
    }

private:
    using argument_table_iterator = basic_argument_table_iterator<TRAITS>;
    using argument_ptr_type = std::shared_ptr<argument_type>;
    using parser_data_type = internal::basic_parser_data<TRAITS>;
    using parser_data_ptr_type = std::shared_ptr<parser_data_type>;
    using results_data_type = internal::basic_results_data<TRAITS>;
    using results_data_ptr_type = std::shared_ptr<results_data_type>;

    bool parse_named_argument(argument_table_iterator& input_iterator, const results_data_ptr_type& results_data_ptr)
    {
        auto& input_value = input_iterator.peek_next();

        auto arg_iter = m_data_ptr->m_names_repository.find(input_value);
        if (arg_iter == m_data_ptr->m_names_repository.end())
        {
            return false;
        }

        input_iterator.take_next();

        auto& arg_object_ptr = arg_iter->second;

        if (arg_object_ptr->is_value_required())
        {
            // TODO: (planned) values parsing not implemented
            throw parse_exception("Not implemented");
        }
        // TODO: (planned) check if multivalue
        else
        {
            // check if already given
            if (results_data_ptr->has_value(arg_object_ptr))
            {
                throw parse_exception("Duplicated argument found");
            }

            results_data_ptr->append_value(arg_object_ptr, TRAITS::get_true_literal());
        }

        return true;
    }

    void parse_positional_argument(
        argument_table_iterator& input_iterator, const results_data_ptr_type& results_data_ptr)
    {
        if (!m_data_ptr->m_positional_arguments_enabled)
        {
            throw parse_exception("Positional arguments not enabled");
        }

        const auto& input_value = input_iterator.take_next();

        results_data_ptr->append_positional_argument(input_value);
    }

    void ensure_not_locked()
    {
        if (m_data_ptr->m_locked)
        {
            throw parser_state_exception("Parsed is locked");
        }
    }

    void check_names(const string_vector_type& names)
    {
        auto name_count = names.size();

        /* check number of names */
        if (name_count < 1)
        {
            throw configuration_exception("No switch names given");
        }

        ensure_names_characters_valid(names);
        ensure_no_duplicated_names(names);
        ensure_names_not_registered(names);
    }

    void ensure_names_not_registered(const string_vector_type& names)
    {
        /* check if names are not already registered */
        for (const auto& name : names)
        {
            if (m_data_ptr->m_names_repository.find(name) != m_data_ptr->m_names_repository.end())
            {
                throw configuration_exception("Argument with given name already registered");
            }
        }
    }

    void ensure_no_duplicated_names(const string_vector_type& names)
    {
        auto name_count = names.size();

        for (std::size_t i = 0; i < name_count; ++i)
        {
            for (std::size_t j = i + 1; j < name_count; ++j)
            {
                if (names[i] == names[j])
                {
                    throw configuration_exception("Duplicated name");
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

    void ensure_names_characters_valid(const string_vector_type& names)
    {
        for (const auto& name : names)
        {
            ensure_name_characters_valid(name);
        }
    }

    void ensure_name_characters_valid(const string_type& name)
    {
        if (name.empty())
        {
            throw configuration_exception("Argument name must not be empty");
        }

        for (auto c : name)
        {
            if (char_utils_type::is_space(c))
            {
                throw configuration_exception("Argument name must not contain whitespace characters");
            }
        }
    }

    const parser_data_ptr_type m_data_ptr;
};

} // namespace args
} // namespace oct

#endif /*OCTARGS_BASIC_PARSER_HPP_*/
