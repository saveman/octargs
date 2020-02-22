#ifndef OCTARGS_ARGUMENT_BASE_HPP_
#define OCTARGS_ARGUMENT_BASE_HPP_

#include "argument.hpp"

namespace oct
{
namespace args
{

template <typename derived_T, typename char_T, typename values_storage_T = internal::null_values_storage>
class basic_argument_base : public basic_argument<char_T, values_storage_T>
{
public:
    using derived_type = derived_T;
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using base_type = basic_argument<char_type, values_storage_type>;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using handler_type = internal::basic_argument_handler<char_type, values_storage_type>;
    using handler_ptr_type = std::shared_ptr<const handler_type>;

    template <typename DATA>
    using type_handler_type = basic_argument_type_handler<DATA, char_type, values_storage_type>;

    template <typename DATA>
    type_handler_type<DATA>& set_type()
    {
        auto handler_ptr = std::make_shared<type_handler_type<DATA>>();

        set_handler(handler_ptr);

        return *handler_ptr;
    }

    template <typename DATA>
    type_handler_type<DATA>& set_type_and_storage(DATA values_storage_type::*member_ptr)
    {
        return set_type<DATA>().set_storage(member_ptr);
    }

    template <typename DATA>
    type_handler_type<DATA>& set_type_and_storage(std::vector<DATA> values_storage_type::*member_ptr)
    {
        return set_type<DATA>().set_storage(member_ptr);
    }

    derived_type& set_description(const string_type& text)
    {
        m_description = text;
        return cast_this_to_derived();
    }

    const string_vector_type& get_names() const final
    {
        return m_names;
    }

    const string_vector_type& get_default_values() const final
    {
        return m_default_values;
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

    const handler_ptr_type& get_handler() const final
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

protected:
    enum flags : std::uint32_t
    {
        FLAG_IS_EXCLUSIVE = (1 << 0),
        FLAG_IS_ASSIGNABLE_BY_NAME = (1 << 1),
        FLAG_IS_ACCEPTING_IMMEDIATE_VALUE = (1 << 2),
        FLAG_IS_ACCEPTING_SEPARATE_VALUE = (1 << 3),
    };

    explicit basic_argument_base(std::uint32_t flags, const string_vector_type& names)
        : base_type()
        , m_flags(flags)
        , m_names(names)
        , m_description()
        , m_min_count(0)
        , m_max_count(1)
        , m_default_values()
        , m_handler_ptr()
    {
        // noop
    }

    derived_type& set_default_values_internal(const string_vector_type& values)
    {
        m_default_values = values;
        return cast_this_to_derived();
    }

    derived_type& set_value_name_internal(const string_type& name)
    {
        m_value_name = name;
        return cast_this_to_derived();
    }

    derived_type& set_min_count(std::size_t count)
    {
        m_min_count = count;
        return cast_this_to_derived();
    }

    derived_type& set_max_count(std::size_t count)
    {
        m_max_count = count;
        return cast_this_to_derived();
    }

    derived_type& set_max_count_unlimited()
    {
        return set_max_count(std::numeric_limits<std::size_t>::max());
    }

private:
    void set_handler(const handler_ptr_type& handler_ptr)
    {
        m_handler_ptr = handler_ptr;
    }

    derived_type& cast_this_to_derived()
    {
        return static_cast<derived_type&>(*this);
    }

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
    /// Defaults values.
    string_vector_type m_default_values;
    /// Values storage handler.
    handler_ptr_type m_handler_ptr;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_BASE_HPP_
