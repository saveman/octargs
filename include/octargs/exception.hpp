#ifndef OCTARGS_EXCEPTION_HPP_
#define OCTARGS_EXCEPTION_HPP_

#include <exception>
#include <memory>
#include <string>

namespace oct
{
namespace args
{

/// \brief String wrapper using shared pointer
///
/// This class wraps std::basic_string stored using shared pointer.
/// It is used to store strings safely in exceptions as shared pointer
/// copy construction does not throw exceptions.
///
/// \tparam char_T      char type (as in std::basic_string)
template <typename char_T>
class basic_shared_string
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    explicit basic_shared_string(const string_type& str)
        : m_string_ptr(std::make_shared<string_type>(str))
    {
        // noop
    }

    operator const string_type&() const
    {
        return *m_string_ptr;
    }

private:
    std::shared_ptr<string_type> m_string_ptr;
};

//---------------------------------

/// \brief Exception thrown when conversion function is missing.
class missing_converter : public std::logic_error
{
public:
    explicit missing_converter()
        : std::logic_error("converter function is missing")
    {
        // noop
    }
};

/// \brief Exception thrown when conversion function is missing.
///
/// \tparam char_T      char type (as in std::basic_string)
template <typename char_T>
class missing_converter_ex : public missing_converter
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    missing_converter_ex(const string_type& value)
        : missing_converter()
        , m_value(value)
    {
        // noop
    }

    const string_type& get_value() const
    {
        return m_value;
    }

private:
    basic_shared_string<char_type> m_value;
};

//---------------------------------

/// \brief Exception thrown when conversion failed
class conversion_error : public std::runtime_error
{
public:
    explicit conversion_error()
        : std::runtime_error("Conversion failed")
    {
        // noop
    }
};

/// \brief Exception thrown when conversion failed
///
/// \tparam char_T      char type (as in std::basic_string)
template <typename char_T>
class conversion_error_ex : public conversion_error
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    explicit conversion_error_ex(const string_type& value)
        : conversion_error()
        , m_value(value)
    {
        // noop
    }

    const string_type& get_value() const
    {
        return m_value;
    }

private:
    basic_shared_string<char_type> m_value;
};

//---------------------------------

/// \brief Exception thrown when unknown argument was requested
class unknown_argument : public std::logic_error
{
public:
    explicit unknown_argument()
        : std::logic_error("Unknown argument requested")
    {
        // noop
    }
};

/// \brief Exception thrown when unknown argument was requested
///
/// \tparam char_T      char type (as in std::basic_string)
template <typename char_T>
class unknown_argument_ex : public unknown_argument
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    explicit unknown_argument_ex(const string_type& name)
        : unknown_argument()
        , m_name(name)
    {
        // noop
    }

    const string_type& get_name() const
    {
        return m_name;
    }

private:
    basic_shared_string<char_type> m_name;
};

//---------------------------------

/// \brief Exception thrown when given argument name is invalid
class invalid_argument_name : public std::logic_error
{
public:
    explicit invalid_argument_name(const std::string& message)
        : std::logic_error(message)
    {
        // noop
    }
};

/// \brief Exception thrown when given argument name is invalid
///
/// \tparam char_T      char type (as in std::basic_string)
template <typename char_T>
class invalid_argument_name_ex : public invalid_argument_name
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    explicit invalid_argument_name_ex(const std::string& message, const string_type& name)
        : invalid_argument_name(message)
        , m_name(name)
    {
        // noop
    }

    const string_type& get_name() const
    {
        return m_name;
    }

private:
    basic_shared_string<char_type> m_name;
};

//---------------------------------

/// \brief Exception thrown when there is a conflict between positional and/or subparser arguments
///
/// \tparam char_T      char type (as in std::basic_string)
class subparser_positional_conflict : public std::logic_error
{
public:
    explicit subparser_positional_conflict(const std::string& message)
        : std::logic_error(message)
    {
        // noop
    }
};

//---------------------------------

/// \brief Exception thrown when given parser name is invalid
class invalid_parser_name : public std::logic_error
{
public:
    explicit invalid_parser_name(const std::string& message)
        : std::logic_error(message)
    {
        // noop
    }
};

/// \brief Exception thrown when given parser name is invalid
///
/// \tparam char_T      char type (as in std::basic_string)
template <typename char_T>
class invalid_parser_name_ex : public invalid_parser_name
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    explicit invalid_parser_name_ex(const std::string& message, const string_type& name)
        : invalid_parser_name(message)
        , m_name(name)
    {
        // noop
    }

    const string_type& get_name() const
    {
        return m_name;
    }

private:
    basic_shared_string<char_type> m_name;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_EXCEPTION_HPP_
