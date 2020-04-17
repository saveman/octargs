#ifndef OCTARGS_VALUED_ARGUMENT_IMPL_HPP_
#define OCTARGS_VALUED_ARGUMENT_IMPL_HPP_

#include "argument_base_impl.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T, typename values_storage_T = null_values_storage>
class basic_valued_argument_impl
    : public basic_argument_base_impl<basic_valued_argument_impl<char_T, values_storage_T>, char_T, values_storage_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using base_type = basic_argument_base_impl<basic_valued_argument_impl<char_type, values_storage_type>, char_type,
        values_storage_type>;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    explicit basic_valued_argument_impl(const string_vector_type& names)
        : base_type(base_type::FLAG_IS_ASSIGNABLE_BY_NAME | base_type::FLAG_IS_ACCEPTING_IMMEDIATE_VALUE
                | base_type::FLAG_IS_ACCEPTING_SEPARATE_VALUE,
            names)
    {
        // noop
    }

    basic_valued_argument_impl& set_default_values(const string_vector_type& values)
    {
        return base_type::set_default_values_internal(values);
    }

    basic_valued_argument_impl& set_default_value(const string_type& value)
    {
        return base_type::set_default_values_internal({ value });
    }

    basic_valued_argument_impl& set_allowed_values(const string_vector_type& values)
    {
        return base_type::set_allowed_values_internal(values);
    }

    basic_valued_argument_impl& set_value_name(const string_type& name)
    {
        return base_type::set_value_name_internal(name);
    }

    basic_valued_argument_impl& set_min_count(std::size_t count)
    {
        return base_type::set_min_count(count);
    }

    basic_valued_argument_impl& set_max_count(std::size_t count)
    {
        return base_type::set_max_count(count);
    }

    basic_valued_argument_impl& set_max_count_unlimited()
    {
        return base_type::set_max_count_unlimited();
    }
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_VALUED_ARGUMENT_IMPL_HPP_
