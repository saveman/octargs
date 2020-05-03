#ifndef OCTARGS_VALUED_ARGUMENT_IMPL_HPP_
#define OCTARGS_VALUED_ARGUMENT_IMPL_HPP_

#include "argument_base_impl.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T, typename values_storage_T>
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

    using parser_data_weak_ptr_type = typename base_type::parser_data_weak_ptr_type;

    explicit basic_valued_argument_impl(parser_data_weak_ptr_type parser_data_ptr, const string_vector_type& names)
        : base_type(parser_data_ptr,
            base_type::FLAG_IS_ASSIGNABLE_BY_NAME | base_type::FLAG_IS_ACCEPTING_IMMEDIATE_VALUE
                | base_type::FLAG_IS_ACCEPTING_SEPARATE_VALUE,
            names)
    {
        // noop
    }

    void set_default_values(const string_vector_type& values)
    {
        base_type::set_default_values_internal(values);
    }

    void set_default_value(const string_type& value)
    {
        base_type::set_default_values_internal({ value });
    }

    void set_allowed_values(const string_vector_type& values)
    {
        base_type::set_allowed_values_internal(values);
    }

    void set_value_name(const string_type& name)
    {
        base_type::set_value_name_internal(name);
    }

    void set_min_count(std::size_t count)
    {
        base_type::set_min_count(count);
    }

    void set_max_count(std::size_t count)
    {
        base_type::set_max_count(count);
    }

    void set_max_count_unlimited()
    {
        base_type::set_max_count_unlimited();
    }
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_VALUED_ARGUMENT_IMPL_HPP_
