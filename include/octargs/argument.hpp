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
#include "parser_dictionary.hpp"

namespace oct
{
namespace args
{

template <typename char_T, typename values_storage_T>
class basic_argument : public internal::basic_argument_tag
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;
    using handler_type = internal::basic_argument_handler<char_type, values_storage_type>;
    using handler_ptr_type = std::shared_ptr<const handler_type>;

    virtual ~basic_argument() = default;

    virtual const string_vector_type& get_names() const = 0;

    virtual const string_vector_type& get_default_values() const = 0;

    virtual std::size_t get_min_count() const = 0;

    virtual std::size_t get_max_count() const = 0;

    virtual bool is_max_count_unlimited() const = 0;

    virtual const handler_ptr_type& get_handler() const = 0;

    virtual const string_type& get_description() const = 0;

    virtual const string_type& get_value_name() const = 0;

    virtual bool is_assignable_by_name() const = 0;

    virtual bool is_accepting_immediate_value() const = 0;

    virtual bool is_accepting_separate_value() const = 0;
};

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
        return static_cast<derived_type&>(*this);
    }

    derived_type& set_min_count(std::size_t count)
    {
        m_min_count = count;
        return static_cast<derived_type&>(*this);
    }

    derived_type& set_max_count(std::size_t count)
    {
        m_max_count = count;
        return static_cast<derived_type&>(*this);
    }

    derived_type& set_max_count_unlimited()
    {
        return set_max_count(std::numeric_limits<std::size_t>::max());
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

protected:
    explicit basic_argument_base(const string_vector_type& names)
        : base_type()
        , m_names(names)
        , m_description()
        , m_min_count(0)
        , m_max_count(1)
        , m_default_values()
        , m_handler_ptr()
    {
        // noop
    }

    void set_default_values_internal(const string_vector_type& values)
    {
        m_default_values = values;
    }

    void set_value_name_internal(const string_type& name)
    {
        m_value_name = name;
    }

private:
    void set_handler(const handler_ptr_type& handler_ptr)
    {
        m_handler_ptr = handler_ptr;
    }

    /// Names.
    const string_vector_type m_names;
    /// Description
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

template <typename char_T, typename values_storage_T = internal::null_values_storage>
class basic_switch_argument
    : public basic_argument_base<basic_switch_argument<char_T, values_storage_T>, char_T, values_storage_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using base_type
        = basic_argument_base<basic_switch_argument<char_type, values_storage_type>, char_type, values_storage_type>;

    using dictionary_type = parser_dictionary<char_type>;
    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    explicit basic_switch_argument(const string_vector_type& names)
        : base_type(names)
    {
        // noop
    }

    bool is_assignable_by_name() const final
    {
        return true;
    }

    bool is_accepting_immediate_value() const final
    {
        return false;
    }

    bool is_accepting_separate_value() const final
    {
        return false;
    }

    basic_switch_argument& set_default_values_count(std::size_t count)
    {
        this->set_default_values_internal(string_vector_type(count, dictionary_type::get_switch_enabled_literal()));
        return *this;
    }
};

template <typename char_T, typename values_storage_T = internal::null_values_storage>
class basic_valued_argument
    : public basic_argument_base<basic_valued_argument<char_T, values_storage_T>, char_T, values_storage_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using base_type
        = basic_argument_base<basic_valued_argument<char_type, values_storage_type>, char_type, values_storage_type>;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    explicit basic_valued_argument(const string_vector_type& names)
        : base_type(names)
    {
        // noop
    }

    bool is_assignable_by_name() const final
    {
        return true;
    }

    bool is_accepting_immediate_value() const final
    {
        return true;
    }

    bool is_accepting_separate_value() const final
    {
        return true;
    }

    basic_valued_argument& set_default_values(const string_vector_type& values)
    {
        this->set_default_values_internal(values);
        return *this;
    }

    basic_valued_argument& set_default_value(const string_type& value)
    {
        this->set_default_values_internal({ value });
        return *this;
    }

    basic_valued_argument& set_value_name(const string_type& name)
    {
        this->set_value_name_internal(name);
        return *this;
    }
};

template <typename char_T, typename values_storage_T = internal::null_values_storage>
class basic_positional_argument
    : public basic_argument_base<basic_positional_argument<char_T, values_storage_T>, char_T, values_storage_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using base_type = basic_argument_base<basic_positional_argument<char_type, values_storage_type>, char_type,
        values_storage_type>;

    using dictionary_type = parser_dictionary<char_type>;
    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    explicit basic_positional_argument(const string_vector_type& names)
        : base_type(names)
    {
        // noop
    }

    basic_positional_argument& set_default_values(const string_vector_type& values)
    {
        this->set_default_values_internal(values);
        return *this;
    }

    basic_positional_argument& set_default_value(const string_type& value)
    {
        this->set_default_values_internal({ value });
        return *this;
    }

    bool is_assignable_by_name() const final
    {
        return false;
    }

    bool is_accepting_immediate_value() const final
    {
        return false;
    }

    bool is_accepting_separate_value() const final
    {
        return false;
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_HPP_
