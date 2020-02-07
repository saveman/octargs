#ifndef OCTARGS_EXCEPTION_HPP_
#define OCTARGS_EXCEPTION_HPP_

#include <exception>
#include <string>

namespace oct
{
namespace args
{
/**
 * Exception: base class for all exceptions.
 */
class exception : public std::exception
{
public:
    virtual const char* what() const noexcept
    {
        return m_message.c_str();
    }

protected:
    exception()
        : m_message()
    {
        // noop
    }

    exception(const std::string& message)
        : m_message(message)
    {
        // noop
    }

private:
    std::string m_message;
};

/**
 * Exception: invalid parser configuration requested.
 */
class configuration_exception : public exception
{
public:
    configuration_exception()
        : exception()
    {
        // noop
    }

    configuration_exception(const std::string& message)
        : exception(message)
    {
        // noop
    }
};

/**
 * Exception: parser is in invalid state.
 */
class parser_state_exception : public exception
{
public:
    parser_state_exception()
        : exception()
    {
        // noop
    }

    parser_state_exception(const std::string& message)
        : exception(message)
    {
        // noop
    }
};

/**
 * Exception: invalid input data given to parser.
 */
class parse_exception : public exception
{
public:
    parse_exception()
        : exception()
    {
        // noop
    }

    parse_exception(const std::string& message)
        : exception(message)
    {
        // noop
    }
};

/**
 * Exception: logic error occurred (e.g. non-existing argument requested).
 */
class logic_error_exception : public exception
{
public:
    logic_error_exception()
        : exception()
    {
        // noop
    }

    logic_error_exception(const std::string& message)
        : exception(message)
    {
        // noop
    }
};

} // namespace args
} // namespace oct

#endif /*OCTARGS_EXCEPTION_HPP_*/
