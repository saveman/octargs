#ifndef OCTARGS_POSITIONAL_ARGUMENT_HPP_
#define OCTARGS_POSITIONAL_ARGUMENT_HPP_

#include "argument_base.hpp"
#include "internal/positional_argument_impl.hpp"

namespace oct
{
namespace args
{

/// \brief Positional argument
///
/// Positional arguments are arguments that are normally given at end of input
/// without providing argument name - for example paths of files to process.
///
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of class uses as a storage for parsed values
/// \tparam data_T              argument value data type
template <typename char_T, typename values_storage_T, typename data_T = void>
class basic_positional_argument : public basic_argument_base<basic_positional_argument,
                                      internal::basic_positional_argument_impl, char_T, values_storage_T, data_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;
    using data_type = data_T;

    using base_type = basic_argument_base<oct::args::basic_positional_argument,
        internal::basic_positional_argument_impl, char_T, values_storage_T, data_T>;

    using string_type = typename base_type::string_type;
    using string_vector_type = typename base_type::string_vector_type;
    using argument_type = typename base_type::argument_type;
    using argument_ptr_type = typename base_type::argument_ptr_type;

    explicit basic_positional_argument(const argument_ptr_type& argument_ptr)
        : base_type(argument_ptr)
    {
        // noop
    }

    template <typename handler_ptr_T>
    explicit basic_positional_argument(const argument_ptr_type& argument_ptr, const handler_ptr_T& handler_ptr)
        : base_type(argument_ptr, handler_ptr)
    {
        // noop
    }

    basic_positional_argument& set_default_values(const string_vector_type& values)
    {
        this->get_argument().set_default_values(values);
        return this->cast_this_to_derived();
    }

    basic_positional_argument& set_default_value(const string_type& value)
    {
        this->get_argument().set_default_value(value);
        return this->cast_this_to_derived();
    }

    basic_positional_argument& set_allowed_values(const string_vector_type& values)
    {
        this->get_argument().set_allowed_values(values);
        return this->cast_this_to_derived();
    }

    basic_positional_argument& set_value_name(const string_type& name)
    {
        this->get_argument().set_value_name(name);
        return this->cast_this_to_derived();
    }

    basic_positional_argument& set_min_count(std::size_t count)
    {
        this->get_argument().set_min_count(count);
        return this->cast_this_to_derived();
    }

    basic_positional_argument& set_max_count(std::size_t count)
    {
        this->get_argument().set_max_count(count);
        return this->cast_this_to_derived();
    }

    basic_positional_argument& set_max_count_unlimited()
    {
        this->get_argument().set_max_count_unlimited();
        return this->cast_this_to_derived();
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_POSITIONAL_ARGUMENT_HPP_
