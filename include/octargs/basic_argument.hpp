#ifndef OCTARGS_BASIC_ARGUMENT_HPP_
#define OCTARGS_BASIC_ARGUMENT_HPP_

#include <limits>
#include <string>

namespace oct
{
namespace args
{

enum class argument_kind
{
    SWITCH,
    VALUED,
    POSITIONAL
};

template <class TRAITS>
class basic_argument
{
public:
    using string_vector = typename TRAITS::string_vector_type;

    basic_argument(argument_kind kind, const string_vector& names, bool is_required)
        : m_kind(kind)
        , m_names(names)
        , m_is_required(is_required)
        , m_max_count(1)
    {
        // noop
    }

    basic_argument& set_max_count(std::size_t count)
    {
        m_max_count = count;
        return *this;
    }

    basic_argument& set_unlimited_count()
    {
        m_max_count = std::numeric_limits<decltype(m_max_count)>::max();
        return *this;
    }

    argument_kind get_kind() const
    {
        return m_kind;
    }

    const string_vector& get_names() const
    {
        return m_names;
    }

    bool is_required() const
    {
        return m_is_required;
    }

    std::size_t get_max_count() const
    {
        return m_max_count;
    }

private:
    /** Argument kind (type). */
    argument_kind m_kind;
    /** Argument names. */
    const string_vector m_names;
    /** Flag: is argument required. */
    bool m_is_required;
    /** Maximum number of occurrences in input. */
    std::size_t m_max_count;
};

} // namespace args
} // namespace oct

#endif /*OCTARGS_BASIC_ARGUMENT_HPP_*/
