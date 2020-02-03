#ifndef OCTARGS_BASIC_ARGUMENT_HPP_
#define OCTARGS_BASIC_ARGUMENT_HPP_

#include <string>

namespace oct
{
namespace args
{
    template <class traits>
    class basic_argument
    {
    public:
        using string_vector = typename traits::string_vector_type;

        basic_argument(const string_vector& names)
            : m_names(names)
        {
            // noop
        }

        const string_vector& get_names() const
        {
            return m_names;
        }

    private:
        string_vector m_names;
    };

} // namespace args
} // namespace oct

#endif /*OCTARGS_BASIC_ARGUMENT_HPP_*/
