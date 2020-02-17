#ifndef OCTARGS_EXCEPTION_HPP_
#define OCTARGS_EXCEPTION_HPP_

#include <exception>
#include <string>

namespace oct
{
namespace args
{

/// Exception: base class for all exceptions.
class exception : public std::exception
{
public:
    virtual const char* what() const noexcept
    {
        return m_message.c_str();
    }

protected:
    explicit exception()
        : m_message()
    {
        // noop
    }

    explicit exception(const std::string& message)
        : m_message(message)
    {
        // noop
    }

private:
    std::string m_message;
};

/// Exception: invalid parser configuration requested.
class configuration_exception : public exception
{
public:
    explicit configuration_exception()
        : exception()
    {
        // noop
    }

    explicit configuration_exception(const std::string& message)
        : exception(message)
    {
        // noop
    }
};

/// Exception: invalid input data given to parser.
class parse_exception : public exception
{
public:
    explicit parse_exception()
        : exception()
    {
        // noop
    }

    explicit parse_exception(const std::string& message)
        : exception(message)
    {
        // noop
    }
};

/// Exception: logic error occurred (e.g. non-existing argument requested).
class logic_error_exception : public exception
{
public:
    explicit logic_error_exception()
        : exception()
    {
        // noop
    }

    explicit logic_error_exception(const std::string& message)
        : exception(message)
    {
        // noop
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_EXCEPTION_HPP_
