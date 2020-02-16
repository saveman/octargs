#ifndef OCTARGS_ARGUMENT_HPP_
#define OCTARGS_ARGUMENT_HPP_

#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "exception.hpp"
#include "internal/misc.hpp"
#include "storage_handler.hpp"

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

template <typename TRAITS, typename VALUES_STORAGE = internal::null_values_storage>
class basic_argument : public internal::basic_argument_tag
{
public:
    using traits_type = TRAITS;
    using values_storage_type = VALUES_STORAGE;
    using string_vector_type = typename TRAITS::string_vector_type;
    using storage_handler_type = storage_handler<TRAITS, VALUES_STORAGE>;
    using storage_handler_ptr_type = std::shared_ptr<const storage_handler_type>;

    template <typename VALUE>
    void set_storage_location(VALUE values_storage_type::*member_ptr)
    {
        using value_type = VALUE;
        using handler_type = member_storage_handler<traits_type, values_storage_type, value_type>;

        set_storage_handler(std::make_shared<handler_type>(member_ptr));
    }

    template <typename VALUE>
    void set_storage_location(std::vector<VALUE> values_storage_type::*member_ptr)
    {
        using value_type = VALUE;
        using container_type = std::vector<VALUE>;
        using handler_type
            = member_container_storage_handler<traits_type, values_storage_type, container_type, value_type>;

        set_storage_handler(std::make_shared<handler_type>(member_ptr));
    }

    argument_kind get_kind() const
    {
        return m_kind;
    }

    const string_vector_type& get_names() const
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

    storage_handler_ptr_type get_storage_handler() const
    {
        return m_storage;
    }

    void set_storage_handler(storage_handler_ptr_type storage)
    {
        m_storage = storage;
    }

protected:
    basic_argument(argument_kind kind, const string_vector_type& names)
        : m_kind(kind)
        , m_names(names)
        , m_is_required(false)
        , m_max_count(1)
    {
        // noop
    }

    void set_unlimited_count()
    {
        set_max_count(std::numeric_limits<std::size_t>::max());
    }

    void set_max_count(std::size_t count)
    {
        m_max_count = count;
    }

    void set_required(bool required)
    {
        m_is_required = required;
    }

private:
    /** Argument kind (type). */
    argument_kind m_kind;
    /** Argument names. */
    const string_vector_type m_names;
    /** Flag: is argument required. */
    bool m_is_required;
    /** Maximum number of occurrences in input. */
    std::size_t m_max_count;
    /** Values storage handler. */
    storage_handler_ptr_type m_storage;
};

template <typename TRAITS, typename VALUES_STORAGE = internal::null_values_storage>
class switch_argument : public basic_argument<TRAITS, VALUES_STORAGE>
{
public:
    using base_type = basic_argument<TRAITS, VALUES_STORAGE>;
    using string_vector_type = typename TRAITS::string_vector_type;

    switch_argument(const string_vector_type& names)
        : base_type(argument_kind::SWITCH, names)
    {
        // noop
    }

    switch_argument& set_max_count(std::size_t count)
    {
        base_type::set_max_count(count);
        return *this;
    }

    switch_argument& set_unlimited_count()
    {
        base_type::set_unlimited_count();
        return *this;
    }
};

template <typename TRAITS, typename VALUES_STORAGE = internal::null_values_storage>
class valued_argument : public basic_argument<TRAITS, VALUES_STORAGE>
{
public:
    using base_type = basic_argument<TRAITS, VALUES_STORAGE>;
    using string_vector = typename TRAITS::string_vector_type;

    valued_argument(const string_vector& names)
        : base_type(argument_kind::VALUED, names)
    {
        // noop
    }

    valued_argument& set_max_count(std::size_t count)
    {
        base_type::set_max_count(count);
        return *this;
    }

    valued_argument& set_unlimited_count()
    {
        base_type::set_unlimited_count();
        return *this;
    }
};

template <typename TRAITS, typename VALUES_STORAGE = internal::null_values_storage>
class positional_argument : public basic_argument<TRAITS, VALUES_STORAGE>
{
public:
    using base_type = basic_argument<TRAITS, VALUES_STORAGE>;
    using string_vector = typename TRAITS::string_vector_type;

    positional_argument(const string_vector& names, bool required, bool multivalue)
        : base_type(argument_kind::POSITIONAL, names)
    {
        base_type::set_required(required);
        if (multivalue)
        {
            base_type::set_unlimited_count();
        }
    }
};

} // namespace args
} // namespace oct

#endif /*OCTARGS_ARGUMENT_HPP_*/
