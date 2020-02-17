#ifndef OCTARGS_ARGUMENT_HPP_
#define OCTARGS_ARGUMENT_HPP_

#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "argument_type_handler.hpp"
#include "exception.hpp"
#include "internal/argument_handler.hpp"
#include "internal/misc.hpp"

namespace oct
{
namespace args
{

template <typename TRAITS, typename VALUES_STORAGE>
class basic_argument : public internal::basic_argument_tag
{
public:
    using traits_type = TRAITS;
    using values_storage_type = VALUES_STORAGE;
    using string_vector_type = typename TRAITS::string_vector_type;
    using handler_type = internal::basic_argument_handler<TRAITS, VALUES_STORAGE>;
    using handler_ptr_type = std::shared_ptr<const handler_type>;

    template <typename DATA>
    using type_handler_type = basic_argument_type_handler<DATA, TRAITS, VALUES_STORAGE>;

    virtual ~basic_argument() = default;

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

    handler_ptr_type get_handler() const
    {
        return m_handler_ptr;
    }

    virtual bool is_assignable_by_name() const = 0;

    virtual bool is_accepting_immediate_value() const = 0;

    virtual bool is_accepting_separate_value() const = 0;

protected:
    basic_argument(const string_vector_type& names)
        : m_names(names)
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
    void set_handler(handler_ptr_type handler_ptr)
    {
        m_handler_ptr = handler_ptr;
    }

    /// Argument names.
    const string_vector_type m_names;
    /// Flag: is argument required.
    bool m_is_required;
    /// Maximum number of occurrences in input.
    std::size_t m_max_count;
    /// Values storage handler.
    handler_ptr_type m_handler_ptr;
};

template <typename TRAITS, typename VALUES_STORAGE = internal::null_values_storage>
class basic_switch_argument : public basic_argument<TRAITS, VALUES_STORAGE>
{
public:
    using base_type = basic_argument<TRAITS, VALUES_STORAGE>;
    using string_vector_type = typename TRAITS::string_vector_type;

    basic_switch_argument(const string_vector_type& names)
        : base_type(names)
    {
        // noop
    }

    bool is_assignable_by_name() const override
    {
        return true;
    }

    bool is_accepting_immediate_value() const override
    {
        return false;
    }

    bool is_accepting_separate_value() const override
    {
        return false;
    }

    basic_switch_argument& set_max_count(std::size_t count)
    {
        base_type::set_max_count(count);
        return *this;
    }

    basic_switch_argument& set_unlimited_count()
    {
        base_type::set_unlimited_count();
        return *this;
    }
};

template <typename TRAITS, typename VALUES_STORAGE = internal::null_values_storage>
class basic_valued_argument : public basic_argument<TRAITS, VALUES_STORAGE>
{
public:
    using base_type = basic_argument<TRAITS, VALUES_STORAGE>;
    using string_vector = typename TRAITS::string_vector_type;

    basic_valued_argument(const string_vector& names)
        : base_type(names)
    {
        // noop
    }

    bool is_assignable_by_name() const override
    {
        return true;
    }

    bool is_accepting_immediate_value() const override
    {
        return true;
    }

    bool is_accepting_separate_value() const override
    {
        return true;
    }

    basic_valued_argument& set_max_count(std::size_t count)
    {
        base_type::set_max_count(count);
        return *this;
    }

    basic_valued_argument& set_unlimited_count()
    {
        base_type::set_unlimited_count();
        return *this;
    }
};

template <typename TRAITS, typename VALUES_STORAGE = internal::null_values_storage>
class basic_positional_argument : public basic_argument<TRAITS, VALUES_STORAGE>
{
public:
    using base_type = basic_argument<TRAITS, VALUES_STORAGE>;
    using string_vector = typename TRAITS::string_vector_type;

    basic_positional_argument(const string_vector& names, bool required, bool multivalue)
        : base_type(names)
    {
        base_type::set_required(required);
        if (multivalue)
        {
            base_type::set_unlimited_count();
        }
    }

    bool is_assignable_by_name() const override
    {
        return false;
    }

    bool is_accepting_immediate_value() const override
    {
        return false;
    }

    bool is_accepting_separate_value() const override
    {
        return false;
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_HPP_
