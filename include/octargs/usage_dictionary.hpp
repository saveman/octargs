#ifndef OCTARGS_USAGE_DICTIONARY_HPP_
#define OCTARGS_USAGE_DICTIONARY_HPP_

#include <string>

#include "exception.hpp"

namespace oct
{
namespace args
{

/// \brief Exception thrown when unsupported key is used with usage dictionary.
class invalid_usage_dictionary_key : public std::logic_error
{
public:
    explicit invalid_usage_dictionary_key(const std::string& message)
        : std::logic_error(message)
    {
        // noop
    }
};

/// \brief Usage dictionary keys
enum class usage_dictionary_string_key
{
    USAGE_LEAD,
    USAGE_NO_ARGUMENTS,
    DEFAULT_NAMED_ARGUMENTS_GROUP_NAME,
    DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME,
    DECORATOR_REQUIRED,
    DECORATOR_MIN_COUNT,
    DECORATOR_MAX_COUNT,
    DECORATOR_MAX_COUNT_UNLIMITED,
    DECORATOR_VALUE_SEPARATOR,
    DECORATOR_DEFAULT,
    DECORATOR_ALLOWED,
};

/// \brief Usage terms dictionary
///
/// \tparam char_T              char type (as in std::basic_string)
template <typename char_T>
class usage_dictionary
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    virtual ~usage_dictionary() = default;
    virtual const string_type& get_string(usage_dictionary_string_key key) const = 0;
};

/// \brief Usage terms dictionary
///
/// \tparam char_T              char type (as in std::basic_string)
template <typename char_T>
class default_usage_dictionary
{
public:
    default_usage_dictionary() = delete;
};

/// \brief Usage terms dictionary (specialization for char type)
///
/// \tparam char_T              char type (as in std::basic_string)
template <>
class default_usage_dictionary<char> : public usage_dictionary<char>
{
public:
    const string_type& get_string(usage_dictionary_string_key key) const override
    {
        static const std::map<usage_dictionary_string_key, string_type> DICTIONARY = {
            { usage_dictionary_string_key::USAGE_LEAD, "Usage" },
            { usage_dictionary_string_key::USAGE_NO_ARGUMENTS, "(no arguments available)" },
            { usage_dictionary_string_key::DEFAULT_NAMED_ARGUMENTS_GROUP_NAME, "Optional arguments" },
            { usage_dictionary_string_key::DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME, "Positional arguments" },
            { usage_dictionary_string_key::DECORATOR_REQUIRED, "required" },
            { usage_dictionary_string_key::DECORATOR_MIN_COUNT, "min" },
            { usage_dictionary_string_key::DECORATOR_MAX_COUNT, "max" },
            { usage_dictionary_string_key::DECORATOR_MAX_COUNT_UNLIMITED, "unlimited" },
            { usage_dictionary_string_key::DECORATOR_VALUE_SEPARATOR, ": " },
            { usage_dictionary_string_key::DECORATOR_DEFAULT, "default" },
            { usage_dictionary_string_key::DECORATOR_ALLOWED, "allowed" },
        };
        auto iter = DICTIONARY.find(key);
        if (iter != DICTIONARY.end())
        {
            return iter->second;
        }
        throw invalid_usage_dictionary_key(std::string("Invalid key: ") + std::to_string(static_cast<int>(key)));
    }
};

/// \brief Usage terms dictionary (specialization for wchar_t type)
///
/// \tparam char_T              char type (as in std::basic_string)
template <>
class default_usage_dictionary<wchar_t> : public usage_dictionary<wchar_t>
{
public:
    const string_type& get_string(usage_dictionary_string_key key) const override
    {
        static const std::map<usage_dictionary_string_key, string_type> DICTIONARY = {
            { usage_dictionary_string_key::USAGE_LEAD, L"Usage" },
            { usage_dictionary_string_key::USAGE_NO_ARGUMENTS, L"(no arguments available)" },
            { usage_dictionary_string_key::DEFAULT_NAMED_ARGUMENTS_GROUP_NAME, L"Optional arguments" },
            { usage_dictionary_string_key::DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME, L"Positional arguments" },
            { usage_dictionary_string_key::DECORATOR_REQUIRED, L"required" },
            { usage_dictionary_string_key::DECORATOR_MIN_COUNT, L"min" },
            { usage_dictionary_string_key::DECORATOR_MAX_COUNT, L"max" },
            { usage_dictionary_string_key::DECORATOR_MAX_COUNT_UNLIMITED, L"unlimited" },
            { usage_dictionary_string_key::DECORATOR_VALUE_SEPARATOR, L": " },
            { usage_dictionary_string_key::DECORATOR_DEFAULT, L"default" },
            { usage_dictionary_string_key::DECORATOR_ALLOWED, L"allowed" },
        };
        auto iter = DICTIONARY.find(key);
        if (iter != DICTIONARY.end())
        {
            return iter->second;
        }
        throw invalid_usage_dictionary_key(std::string("Invalid key: ") + std::to_string(static_cast<int>(key)));
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_USAGE_DICTIONARY_HPP_
