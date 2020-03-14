#ifndef OCTARGS_PARSER_ERROR_HPP_
#define OCTARGS_PARSER_ERROR_HPP_

#include "exception.hpp"

namespace oct
{
namespace args
{

/// \brief Parser error code
///
/// Describes the reason why parsing process failed.
enum class parser_error_code
{
    /// Conversion of value to requested type failed.
    CONVERSION_FAILED,
    /// Arguments was specified too many times.
    TOO_MANY_OCCURRENCES,
    /// Arguments were specified but cannot be processed (e.g. too many values specified in input).
    SYNTAX_ERROR,
    /// Argument requires value but it was missing.
    VALUE_MISSING,
    /// Argument does not allow to specify value but one was given.
    UNEXPECTED_VALUE,
    /// Argument is required but was not provided in input.
    REQUIRED_ARGUMENT_MISSING,
    /// Subparser name is required but was not provided in input.
    SUBPARSER_NAME_MISSING,
    /// Subparser with name specified in input was not found.
    SUBPARSER_NOT_FOUND,
    /// Value specified for an argument is not in the allowed values set.
    VALUE_NOT_ALLOWED,
};

/// \brief Exception thrown when parsing cannot be performed.
class parser_error : public std::runtime_error
{
public:
    explicit parser_error(parser_error_code code)
        : std::runtime_error("parsing error occurred")
        , m_error_code(code)
    {
        // noop
    }

    parser_error_code get_error_code() const
    {
        return m_error_code;
    }

private:
    parser_error_code m_error_code;
};

/// \brief Exception thrown when parsing cannot be performed.
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

    const string_type& get_name() const
    {
        return m_name;
    }

    const string_type& get_value() const
    {
        return m_value;
    }

private:
    basic_shared_string<char_type> m_name;
    basic_shared_string<char_type> m_value;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_ERROR_HPP_
