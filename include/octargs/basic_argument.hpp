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

    basic_argument(const string_vector& names, bool value_required)
        : m_names(names)
        , m_value_required(value_required)
    {
        // noop
    }

    const string_vector& get_names() const
    {
        return m_names;
    }

    bool is_value_required() const
    {
        return m_value_required;
    }

private:
    const string_vector m_names;
    const bool m_value_required;
};

} // namespace args
} // namespace oct

#endif /*OCTARGS_BASIC_ARGUMENT_HPP_*/
