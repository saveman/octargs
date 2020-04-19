#ifndef OCTARGS_ARGUMENT_BASE_IMPL_HPP_
#define OCTARGS_ARGUMENT_BASE_IMPL_HPP_

#include "argument.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename derived_T, typename char_T, typename values_storage_T>
class basic_argument_base_impl : public basic_argument<char_T, values_storage_T>
{
public:
    using derived_type = derived_T;
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using base_type = basic_argument<char_type, values_storage_type>;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using handler_type = basic_argument_handler<char_type, values_storage_type>;
    using const_handler_ptr_type = std::shared_ptr<const handler_type>;

    const string_type& get_first_name() const final
    {
        return m_names[0];
    }

    const string_vector_type& get_names() const final
    {
        return m_names;
    }

    const string_vector_type& get_default_values() const final
    {
        return m_default_values;
    }

    const string_vector_type& get_allowed_values() const final
    {
        return m_allowed_values;
    }

    std::size_t get_min_count() const final
    {
        return m_min_count;
    }

    std::size_t get_max_count() const final
    {
        return m_max_count;
    }

    bool is_max_count_unlimited() const final
    {
        return get_max_count() == std::numeric_limits<std::size_t>::max();
    }

    const const_handler_ptr_type& get_handler() const final
    {
        return m_handler_ptr;
    }

    const string_type& get_description() const final
    {
        return m_description;
    }

    const string_type& get_value_name() const final
    {
        return m_value_name;
    }

    bool is_exclusive() const final
    {
        return (m_flags & FLAG_IS_EXCLUSIVE);
    }

    bool is_assignable_by_name() const final
    {
        return (m_flags & FLAG_IS_ASSIGNABLE_BY_NAME);
    }

    bool is_accepting_immediate_value() const final
    {
        return (m_flags & FLAG_IS_ACCEPTING_IMMEDIATE_VALUE);
    }

    bool is_accepting_separate_value() const final
    {
        return (m_flags & FLAG_IS_ACCEPTING_SEPARATE_VALUE);
    }

    void set_description(const string_type& text)
    {
        m_description = text;
    }

    void set_handler(const const_handler_ptr_type& handler_ptr)
    {
        m_handler_ptr = handler_ptr;
    }

protected:
    enum flags : std::uint32_t
    {
        FLAG_IS_EXCLUSIVE = (1 << 0),
        FLAG_IS_ASSIGNABLE_BY_NAME = (1 << 1),
        FLAG_IS_ACCEPTING_IMMEDIATE_VALUE = (1 << 2),
        FLAG_IS_ACCEPTING_SEPARATE_VALUE = (1 << 3),
    };

    explicit basic_argument_base_impl(std::uint32_t flags, const string_vector_type& names)
        : base_type()
        , m_flags(flags)
        , m_names(names)
        , m_description()
        , m_min_count(0)
        , m_max_count(1)
        , m_default_values()
        , m_allowed_values()
        , m_handler_ptr()
    {
        // noop
    }

    void set_default_values_internal(const string_vector_type& values)
    {
        m_default_values = values;
    }

    void set_allowed_values_internal(const string_vector_type& values)
    {
        m_allowed_values = values;
    }

    void set_value_name_internal(const string_type& name)
    {
        m_value_name = name;
    }

    void set_min_count(std::size_t count)
    {
        m_min_count = count;
    }

    void set_max_count(std::size_t count)
    {
        m_max_count = count;
    }

    void set_max_count_unlimited()
    {
        set_max_count(std::numeric_limits<std::size_t>::max());
    }

private:
    /// Flags
    std::uint32_t m_flags;
    /// Names.
    const string_vector_type m_names;
    /// Description.
    string_type m_description;
    /// Value name.
    string_type m_value_name;
    /// Minimum number of occurrences in input.
    std::size_t m_min_count;
    /// Maximum number of occurrences in input.
    std::size_t m_max_count;
    /// Default values.
    string_vector_type m_default_values;
    /// Allowed values.
    string_vector_type m_allowed_values;
    /// Values storage handler.
    const_handler_ptr_type m_handler_ptr;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_BASE_IMPL_HPP_
