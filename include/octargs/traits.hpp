#ifndef OCTARGS_TRAITS_HPP_
#define OCTARGS_TRAITS_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "exception.hpp"

namespace oct
{
namespace args
{

enum class usage_dict_key
{
    DEFAULT_NAMED_ARGUMENTS_GROUP_NAME,
    DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME,
    DECORATOR_REQUIRED,
    DECORATOR_MIN_COUNT,
    DECORATOR_MAX_COUNT,
    DECORATOR_MAX_COUNT_UNLIMITED,
    DECORATOR_VALUE_SEPARATOR,
};

template <typename CHAR_TYPE>
class traits
{
    // noop
};

template <>
class traits<char>
{
public:
    using char_type = char;
    using string_type = std::string;
    using string_vector_type = std::vector<string_type>;
    using ostream_type = std::ostream;

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

    static const string_type& get_usage_literal(usage_dict_key key)
    {
        static const std::map<usage_dict_key, string_type> DICTIONARY = {
            { usage_dict_key::DEFAULT_NAMED_ARGUMENTS_GROUP_NAME, "Options" },
            { usage_dict_key::DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME, "Arguments" },
            { usage_dict_key::DECORATOR_REQUIRED, "required" },
            { usage_dict_key::DECORATOR_MIN_COUNT, "min" },
            { usage_dict_key::DECORATOR_MAX_COUNT, "max" },
            { usage_dict_key::DECORATOR_MAX_COUNT_UNLIMITED, "unlimited" },
            { usage_dict_key::DECORATOR_VALUE_SEPARATOR, ": " },
        };
        auto iter = DICTIONARY.find(key);
        if (iter != DICTIONARY.end())
        {
            return iter->second;
        }
        throw logic_error_exception("Invalid key");
    }

    template <typename DATA>
    static string_type to_string(const DATA& data)
    {
        return std::to_string(data);
    }
};

template <>
class traits<wchar_t>
{
public:
    using char_type = wchar_t;
    using string_type = std::wstring;
    using string_vector_type = std::vector<string_type>;
    using ostream_type = std::wostream;

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

    static const string_type& get_usage_literal(usage_dict_key key)
    {
        static const std::map<usage_dict_key, string_type> DICTIONARY = {
            { usage_dict_key::DEFAULT_NAMED_ARGUMENTS_GROUP_NAME, L"Options" },
            { usage_dict_key::DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME, L"Arguments" },
            { usage_dict_key::DECORATOR_REQUIRED, L"required" },
            { usage_dict_key::DECORATOR_MIN_COUNT, L"min" },
            { usage_dict_key::DECORATOR_MAX_COUNT, L"max" },
            { usage_dict_key::DECORATOR_MAX_COUNT_UNLIMITED, L"unlimited" },
            { usage_dict_key::DECORATOR_VALUE_SEPARATOR, L": " },
        };
        auto iter = DICTIONARY.find(key);
        if (iter != DICTIONARY.end())
        {
            return iter->second;
        }
        throw logic_error_exception("Invalid key");
    }

    template <typename DATA>
    static string_type to_string(const DATA& data)
    {
        return std::to_wstring(data);
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_TRAITS_HPP_
