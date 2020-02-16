#ifndef OCTARGS_TRAITS_HPP_
#define OCTARGS_TRAITS_HPP_

#include <string>
#include <vector>

namespace oct
{
namespace args
{
template <typename CHAR_TYPE>
class traits
{
    // nothing
};

template <>
class traits<char>
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
        static const string_vector_type TRUE_LITERALS {
            "false",
            "0",
            "no",
        };
        return TRUE_LITERALS;
    }

    static char_type get_equal_literal()
    {
        return '=';
    }
};

template <>
class traits<wchar_t>
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
        static const string_vector_type TRUE_LITERALS {
            L"false",
            L"0",
            L"no",
        };
        return TRUE_LITERALS;
    }

    static char_type get_equal_literal()
    {
        return L'=';
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_TRAITS_HPP_
