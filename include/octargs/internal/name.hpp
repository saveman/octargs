#ifndef OCTARGS_NAME_HPP_
#define OCTARGS_NAME_HPP_

#include <string>

#include "string_utils.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T>
class name
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    name()
        : m_name()
    {
        // noop
    }

    name(const string_type& name)
        : m_name(name)
    {
        // noop
    }

    const string_type& get() const
    {
        return m_name;
    }

private:
    string_type m_name;
};

template <typename char_T>
class name_less
{
public:
    using char_type = char_T;
    using name_type = name<char_type>;

    name_less(bool case_sensitive = true)
        : m_internal_comparator(case_sensitive)
    {
        // noop
    }

    bool operator()(const name_type& name1, const name_type& name2) const
    {
        return m_internal_comparator(name1.get(), name2.get());
    }

private:
    string_less<char_type> m_internal_comparator;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_NAME_HPP_
