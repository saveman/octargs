#ifndef OCTARGS_EXCEPTION_HPP
#define OCTARGS_EXCEPTION_HPP

#include <exception>
#include <string>

namespace oct
{
namespace args
{
    class exception : public std::exception
    {
    public:
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

        virtual const char* what() const noexcept
        {
            return m_message.c_str();
        }

    private:
        std::string m_message;
    };

} // namespace args
} // namespace oct

#endif /*OCTARGS_EXCEPTION_HPP*/
