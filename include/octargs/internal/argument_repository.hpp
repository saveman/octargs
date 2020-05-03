#ifndef OCTARGS_PARSER_ARGUMENT_REPOSITORY_HPP_
#define OCTARGS_PARSER_ARGUMENT_REPOSITORY_HPP_

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "argument.hpp"
#include "exclusive_argument_impl.hpp"
#include "name_checker.hpp"
#include "positional_argument_impl.hpp"
#include "string_utils.hpp"
#include "subparser_argument_impl.hpp"
#include "switch_argument_impl.hpp"
#include "valued_argument_impl.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T, typename values_storage_T>
class basic_parser_data;

template <typename char_T, typename values_storage_T>
class basic_argument_repository
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using dictionary_type = dictionary<char_type>;
    using const_dictionary_ptr_type = std::shared_ptr<const dictionary_type>;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;
    using string_less_type = string_less<char_type>;

    using argument_type = basic_argument<char_type, values_storage_type>;
    using const_argument_ptr_type = std::shared_ptr<const argument_type>;

    using exclusive_argument_type = basic_exclusive_argument_impl<char_type, values_storage_type>;
    using exclusive_argument_ptr_type = std::shared_ptr<exclusive_argument_type>;
    using switch_argument_type = basic_switch_argument_impl<char_type, values_storage_type>;
    using switch_argument_ptr_type = std::shared_ptr<switch_argument_type>;
    using valued_argument_type = basic_valued_argument_impl<char_type, values_storage_type>;
    using valued_argument_ptr_type = std::shared_ptr<valued_argument_type>;
    using positional_argument_type = basic_positional_argument_impl<char_type, values_storage_type>;
    using positional_argument_ptr_type = std::shared_ptr<positional_argument_type>;
    using subparser_argument_type = basic_subparser_argument_impl<char_type, values_storage_type>;
    using subparser_argument_ptr_type = std::shared_ptr<subparser_argument_type>;
    using const_subparser_argument_ptr_type = std::shared_ptr<const subparser_argument_type>;

    using parser_data_type = basic_parser_data<char_type, values_storage_type>;
    using parser_data_weak_ptr_type = std::weak_ptr<parser_data_type>;

    basic_argument_repository(const_dictionary_ptr_type dictionary)
        : m_dictionary(check_dictionary(dictionary))
        , m_arguments()
        , m_subparsers_argument()
        , m_names_repository(string_less_type(dictionary->is_case_sensitive()))
    {
        // noop
    }

    exclusive_argument_ptr_type add_exclusive(
        parser_data_weak_ptr_type parser_data_ptr, const string_vector_type& names)
    {
        check_names(names);

        auto new_argument = std::make_shared<exclusive_argument_type>(parser_data_ptr, names);

        add_to_names_repository(new_argument);
        m_arguments.emplace_back(new_argument);

        return new_argument;
    }

    switch_argument_ptr_type add_switch(parser_data_weak_ptr_type parser_data_ptr, const string_vector_type& names)
    {
        check_names(names);

        auto new_argument = std::make_shared<switch_argument_type>(parser_data_ptr, names);

        add_to_names_repository(new_argument);
        m_arguments.emplace_back(new_argument);

        return new_argument;
    }

    valued_argument_ptr_type add_valued(parser_data_weak_ptr_type parser_data_ptr, const string_vector_type& names)
    {
        check_names(names);

        auto new_argument = std::make_shared<valued_argument_type>(parser_data_ptr, names);

        add_to_names_repository(new_argument);
        m_arguments.emplace_back(new_argument);

        return new_argument;
    }

    positional_argument_ptr_type add_positional(parser_data_weak_ptr_type parser_data_ptr, const string_type& name)
    {
        auto names = { name };

        check_names(names);
        if (m_subparsers_argument)
        {
            throw subparser_positional_conflict("subparser argument already registered");
        }

        auto new_argument = std::make_shared<positional_argument_type>(parser_data_ptr, names);

        add_to_names_repository(new_argument);
        m_arguments.emplace_back(new_argument);

        return new_argument;
    }

    subparser_argument_ptr_type add_subparsers(parser_data_weak_ptr_type parser_data_ptr, const string_type& name)
    {
        auto names = { name };

        check_names(names);
        if (m_subparsers_argument)
        {
            throw subparser_positional_conflict("subparser argument already registered");
        }
        if (has_positional_arguments())
        {
            throw subparser_positional_conflict("positional arguments already registered");
        }

        auto new_argument = std::make_shared<subparser_argument_type>(parser_data_ptr, names);

        add_to_names_repository(new_argument);
        m_subparsers_argument = new_argument;

        return new_argument;
    }

private:
    static const const_dictionary_ptr_type& check_dictionary(const const_dictionary_ptr_type& dictionary)
    {
        if (!dictionary)
        {
            throw std::invalid_argument("dictionary");
        }
        return dictionary;
    }

    bool has_positional_arguments() const
    {
        for (auto& argument : m_arguments)
        {
            if (!argument->is_assignable_by_name())
            {
                return true;
            }
        }

        return false;
    }

    void add_to_names_repository(const std::shared_ptr<argument_type>& argument)
    {
        for (const auto& name : argument->get_names())
        {
            m_names_repository.emplace(name, argument);
        }
    }

    void ensure_names_not_registered(const string_vector_type& names)
    {
        for (const auto& name : names)
        {
            if (m_names_repository.find(name) != m_names_repository.end())
            {
                throw invalid_argument_name_ex<char_type>("argument with given name already registered", name);
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

        name_checker<char_type>::ensure_names_valid(names, m_dictionary);
        ensure_names_not_registered(names);
    }

public:
    const_dictionary_ptr_type m_dictionary;
    std::vector<const_argument_ptr_type> m_arguments;
    const_subparser_argument_ptr_type m_subparsers_argument;
    std::map<string_type, const_argument_ptr_type, string_less_type> m_names_repository;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_ARGUMENT_REPOSITORY_HPP_
