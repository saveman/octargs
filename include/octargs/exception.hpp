#ifndef OCTARGS_EXCEPTION_HPP_
#define OCTARGS_EXCEPTION_HPP_

#include <exception>
#include <memory>
#include <string>

namespace oct
{
namespace args
{

template <typename char_T>
class basic_shared_string
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    basic_shared_string(const string_type& str)
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

class missing_converter : public std::logic_error
{
public:
    missing_converter()
        : std::logic_error("converter function is missing")
    {
        // noop
    }
};

//---------------------------------

class conversion_error : public std::runtime_error
{
public:
    conversion_error()
        : std::runtime_error("Conversion failed")
    {
        // noop
    }
};

template <class char_T>
class conversion_error_ex : public conversion_error
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    conversion_error_ex(const string_type& value)
        : conversion_error()
        , m_value(value)
    {
        // noop
    }

    const string_type& value() const
    {
        return m_value;
    }

private:
    basic_shared_string<char_type> m_value;
};

//---------------------------------

class unknown_argument : public std::logic_error
{
public:
    unknown_argument()
        : std::logic_error("Unknown argument requested")
    {
        // noop
    }
};

template <class char_T>
class unknown_argument_ex : public unknown_argument
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    unknown_argument_ex(const string_type& name)
        : unknown_argument()
        , m_name(name)
    {
        // noop
    }

    const string_type& name() const
    {
        return m_name;
    }

private:
    basic_shared_string<char_type> m_name;
};

//---------------------------------

class invalid_argument_name : public std::logic_error
{
public:
    invalid_argument_name(const std::string& message)
        : std::logic_error(message)
    {
        // noop
    }
};

template <class char_T>
class invalid_argument_name_ex : public invalid_argument_name
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    invalid_argument_name_ex(const std::string& message, const string_type& name)
        : invalid_argument_name(message)
        , m_name(name)
    {
        // noop
    }

    const string_type& name() const
    {
        return m_name;
    }

private:
    basic_shared_string<char_type> m_name;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_EXCEPTION_HPP_
