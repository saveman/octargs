#ifndef OCTARGS_BASIC_ARGUMENT_HPP_
#define OCTARGS_BASIC_ARGUMENT_HPP_

#include <string>

namespace oct
{
namespace args
{
template <class TRAITS>
class basic_argument
{
public:
    using string_vector = typename TRAITS::string_vector_type;

    basic_argument(const string_vector& names, bool has_value)
        : m_names(names)
        , m_has_value(has_value)
    {
        // noop
    }

    const string_vector& get_names() const
    {
        return m_names;
    }

    bool has_value() const
    {
        return m_has_value;
    }

private:
    const string_vector m_names;
    const bool m_has_value;
};

} // namespace args
} // namespace oct

#endif /*OCTARGS_BASIC_ARGUMENT_HPP_*/
