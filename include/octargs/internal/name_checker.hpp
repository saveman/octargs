#ifndef OCTARGS_NAME_CHECKER_HPP_
#define OCTARGS_NAME_CHECKER_HPP_

#include <memory>
#include <string>
#include <vector>

#include "../dictionary.hpp"
#include "string_utils.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T>
class name_checker
{
public:
    using char_type = char_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using dictionary_type = dictionary<char_type>;
    using const_dictionary_ptr_type = std::shared_ptr<const dictionary_type>;

    static void ensure_name_valid(const string_type& name, const const_dictionary_ptr_type& dictionary)
    {
        ensure_name_characters_valid(name, dictionary);
    }

    static void ensure_names_valid(const string_vector_type& names, const const_dictionary_ptr_type& dictionary)
    {
        ensure_names_characters_valid(names, dictionary);
        ensure_no_duplicated_names(names, dictionary);
    }

private:
    name_checker() = delete;

    static void ensure_names_characters_valid(
        const string_vector_type& names, const const_dictionary_ptr_type& dictionary)
    {
        for (const auto& name : names)
        {
            ensure_name_characters_valid(name, dictionary);
        }
    }

    static void ensure_name_characters_valid(const string_type& name, const const_dictionary_ptr_type& dictionary)
    {
        if (name.empty())
        {
            throw invalid_argument_name_ex<char_type>("argument name must not be empty", name);
        }

        for (auto c : name)
        {
            if (is_space(c))
            {
                throw invalid_argument_name_ex<char_type>("argument name must not contain whitespace characters", name);
            }
        }

        auto& value_separator = dictionary->get_value_separator_literal();
        if (name.find(value_separator) != string_type::npos)
        {
            throw invalid_argument_name_ex<char_type>("argument name must not contain value separator literal", name);
        }

        auto& subparser_separator = dictionary->get_subparser_separator_literal();
        if (name.find(subparser_separator) != string_type::npos)
        {
            throw invalid_argument_name_ex<char_type>(
                "argument name must not contain subparser separator literal", name);
        }
    }

    static void ensure_no_duplicated_names(const string_vector_type& names, const const_dictionary_ptr_type& dictionary)
    {
        string_less<char_type> comparator(dictionary->is_case_sensitive());

        auto name_count = names.size();

        for (std::size_t i = 0; i < name_count; ++i)
        {
            for (std::size_t j = i + 1; j < name_count; ++j)
            {
                const string_type& name1 = names[i];
                const string_type& name2 = names[j];

                if ((comparator(name1, name2) == false) && (comparator(name2, name1) == false))
                {
                    throw invalid_argument_name_ex<char_type>("duplicated name", names[i]);
                }
            }
        }
    }
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_NAME_CHECKER_HPP_
