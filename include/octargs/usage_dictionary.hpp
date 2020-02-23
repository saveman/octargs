#ifndef OCTARGS_USAGE_DICTIONARY_HPP_
#define OCTARGS_USAGE_DICTIONARY_HPP_

#include <string>

#include "exception.hpp"

namespace oct
{
namespace args
{

class invalid_usage_dictionary_key : public std::logic_error
{
public:
    invalid_usage_dictionary_key(const std::string& message)
        : std::logic_error(message)
    {
        // noop
    }
};

enum class usage_dictionary_string_key
{
    USAGE_DEFAULT_NAMED_ARGUMENTS_GROUP_NAME,
    USAGE_DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME,
    USAGE_DECORATOR_REQUIRED,
    USAGE_DECORATOR_MIN_COUNT,
    USAGE_DECORATOR_MAX_COUNT,
    USAGE_DECORATOR_MAX_COUNT_UNLIMITED,
    USAGE_DECORATOR_VALUE_SEPARATOR,
};

template <typename CHAR_TYPE>
class usage_dictionary
{
public:
    using char_type = CHAR_TYPE;
    using string_type = std::basic_string<char_type>;

    virtual ~usage_dictionary() = default;
    virtual const string_type& get_string(usage_dictionary_string_key key) = 0;
};

template <typename CHAR_TYPE>
class default_usage_dictionary
{
    // noop
};

template <>
class default_usage_dictionary<char> : public usage_dictionary<char>
{
public:
    const string_type& get_string(usage_dictionary_string_key key) override
    {
        static const std::map<usage_dictionary_string_key, string_type> DICTIONARY = {
            { usage_dictionary_string_key::USAGE_DEFAULT_NAMED_ARGUMENTS_GROUP_NAME, "Options" },
            { usage_dictionary_string_key::USAGE_DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME, "Arguments" },
            { usage_dictionary_string_key::USAGE_DECORATOR_REQUIRED, "required" },
            { usage_dictionary_string_key::USAGE_DECORATOR_MIN_COUNT, "min" },
            { usage_dictionary_string_key::USAGE_DECORATOR_MAX_COUNT, "max" },
            { usage_dictionary_string_key::USAGE_DECORATOR_MAX_COUNT_UNLIMITED, "unlimited" },
            { usage_dictionary_string_key::USAGE_DECORATOR_VALUE_SEPARATOR, ": " },
        };
        auto iter = DICTIONARY.find(key);
        if (iter != DICTIONARY.end())
        {
            return iter->second;
        }
        throw invalid_usage_dictionary_key(std::string("Invalid key: ") + std::to_string(static_cast<int>(key)));
    }
};

template <>
class default_usage_dictionary<wchar_t> : public usage_dictionary<wchar_t>
{
public:
    const string_type& get_string(usage_dictionary_string_key key) override
    {
        static const std::map<usage_dictionary_string_key, string_type> DICTIONARY = {
            { usage_dictionary_string_key::USAGE_DEFAULT_NAMED_ARGUMENTS_GROUP_NAME, L"Options" },
            { usage_dictionary_string_key::USAGE_DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME, L"Arguments" },
            { usage_dictionary_string_key::USAGE_DECORATOR_REQUIRED, L"required" },
            { usage_dictionary_string_key::USAGE_DECORATOR_MIN_COUNT, L"min" },
            { usage_dictionary_string_key::USAGE_DECORATOR_MAX_COUNT, L"max" },
            { usage_dictionary_string_key::USAGE_DECORATOR_MAX_COUNT_UNLIMITED, L"unlimited" },
            { usage_dictionary_string_key::USAGE_DECORATOR_VALUE_SEPARATOR, L": " },
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
