#ifndef OCTARGS_PARSER_DICTIONARY_HPP_
#define OCTARGS_PARSER_DICTIONARY_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "exception.hpp"

namespace oct
{
namespace args
{

template <typename CHAR_TYPE>
class parser_dictionary
{
    // noop
};

template <>
class parser_dictionary<char>
{
public:
    using char_type = char;
    using string_type = std::string;
    using string_vector_type = std::vector<string_type>;

    static const string_type& get_switch_enabled_literal()
    {
        static const string_type TRUE_LITERAL { "true" };
        return TRUE_LITERAL;
    }

    static const string_vector_type& get_true_literals()
    {
        static const string_vector_type TRUE_LITERALS {
            "true",
            "1",
            "yes",
        };
        return TRUE_LITERALS;
    }

    static const string_vector_type& get_false_literals()
    {
        static const string_vector_type FALSE_LITERALS {
            "false",
            "0",
            "no",
        };
        return FALSE_LITERALS;
    }

    static char_type get_equal_literal()
    {
        return '=';
    }
};

template <>
class parser_dictionary<wchar_t>
{
public:
    using char_type = wchar_t;
    using string_type = std::wstring;
    using string_vector_type = std::vector<string_type>;

    static const string_type& get_switch_enabled_literal()
    {
        static const string_type TRUE_LITERAL { L"true" };
        return TRUE_LITERAL;
    }

    static const string_vector_type& get_true_literals()
    {
        static const string_vector_type TRUE_LITERALS {
            L"true",
            L"1",
            L"yes",
        };
        return TRUE_LITERALS;
    }

    static const string_vector_type& get_false_literals()
    {
        static const string_vector_type FALSE_LITERALS {
            L"false",
            L"0",
            L"no",
        };
        return FALSE_LITERALS;
    }

    static char_type get_equal_literal()
    {
        return L'=';
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_DICTIONARY_HPP_
