#ifndef OCTARGS_PARSER_DICTIONARY_HPP_
#define OCTARGS_PARSER_DICTIONARY_HPP_

#include <string>
#include <vector>

namespace oct
{
namespace args
{

/// \brief Parser literals dictionary
///
/// \tparam char_T              char type (as in std::basic_string)
template <typename char_T>
class parser_dictionary
{
public:
    using char_type = char_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    virtual ~parser_dictionary() = default;

    virtual const string_type& get_switch_enabled_literal() const = 0;
    virtual const string_vector_type& get_true_literals() const = 0;
    virtual const string_vector_type& get_false_literals() const = 0;
    virtual char_type get_equal_literal() const = 0;
};

/// \brief Parser literals dictionary
///
/// \tparam char_T              char type (as in std::basic_string)
template <typename char_T>
class default_parser_dictionary
{
public:
    default_parser_dictionary() = delete;
};

/// \brief Parser literals dictionary (specialization for char type)
///
template <>
class default_parser_dictionary<char> : public parser_dictionary<char>
{
public:
    const string_type& get_switch_enabled_literal() const override
    {
        static const string_type TRUE_LITERAL { "true" };
        return TRUE_LITERAL;
    }

    const string_vector_type& get_true_literals() const override
    {
        static const string_vector_type TRUE_LITERALS {
            "true",
            "1",
            "yes",
        };
        return TRUE_LITERALS;
    }

    const string_vector_type& get_false_literals() const override
    {
        static const string_vector_type FALSE_LITERALS {
            "false",
            "0",
            "no",
        };
        return FALSE_LITERALS;
    }

    char_type get_equal_literal() const override
    {
        return '=';
    }
};

/// \brief Parser literals dictionary (specialization for wchar_t type)
///
template <>
class default_parser_dictionary<wchar_t> : public parser_dictionary<wchar_t>
{
public:
    const string_type& get_switch_enabled_literal() const override
    {
        static const string_type TRUE_LITERAL { L"true" };
        return TRUE_LITERAL;
    }

    const string_vector_type& get_true_literals() const override
    {
        static const string_vector_type TRUE_LITERALS {
            L"true",
            L"1",
            L"yes",
        };
        return TRUE_LITERALS;
    }

    const string_vector_type& get_false_literals() const override
    {
        static const string_vector_type FALSE_LITERALS {
            L"false",
            L"0",
            L"no",
        };
        return FALSE_LITERALS;
    }

    char_type get_equal_literal() const override
    {
        return L'=';
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_DICTIONARY_HPP_
