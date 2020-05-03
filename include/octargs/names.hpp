#ifndef OCTARGS_NAMES_HPP_
#define OCTARGS_NAMES_HPP_

#include <initializer_list>
#include <string>
#include <vector>

namespace oct
{
namespace args
{

/// \brief Single name "wrapper"
template <typename char_T>
using single_name_wrapper = std::basic_string<char_T>;

/// \brief Multiple names wrapper
template <typename char_T>
class multi_name_wrapper
{
public:
    using char_type = char_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    multi_name_wrapper(const char_type* name_ptr)
        : multi_name_wrapper(string_type(name_ptr))
    {
        // noop
    }

    multi_name_wrapper(const string_type& name)
        : m_names({ name })
    {
        // noop
    }

    multi_name_wrapper(const string_vector_type& names)
        : m_names(names)
    {
        // noop
    }

    multi_name_wrapper(std::initializer_list<string_type> names)
        : m_names(names)
    {
        // noop
    }

    operator const string_vector_type&() const
    {
        return m_names;
    }

private:
    const string_vector_type m_names;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_NAMES_HPP_
