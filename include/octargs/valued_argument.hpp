#ifndef OCTARGS_VALUED_ARGUMENT_HPP_
#define OCTARGS_VALUED_ARGUMENT_HPP_

#include <type_traits>

#include "argument_base.hpp"
#include "converter.hpp"
#include "internal/valued_argument_impl.hpp"

namespace oct
{
namespace args
{

/// \brief Valued argument
///
/// Value argument is an argument that not only have a name but could
/// also accept a value - for example it could be "--lines" argument accepting
/// the number of lines provided by the user, or "--host" argument that is used
/// to specify name of the server to which connection should be made.
///
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of class uses as a storage for parsed values
template <typename char_T, typename values_storage_T, typename data_T = void>
class basic_valued_argument
    : public basic_argument_base<basic_valued_argument<char_T, values_storage_T, data_T>,
          internal::basic_valued_argument_impl<char_T, values_storage_T>, char_T, values_storage_T, data_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;
    using data_type = data_T;

    using base_type = basic_argument_base<basic_valued_argument<char_T, values_storage_T, data_T>,
        internal::basic_valued_argument_impl<char_T, values_storage_T>, char_T, values_storage_T, data_T>;

    using string_type = typename base_type::string_type;
    using string_vector_type = typename base_type::string_vector_type;
    using argument_ptr_type = typename base_type::argument_ptr_type;

    template <typename new_data_T>
    using casted_argument_type = basic_valued_argument<char_type, values_storage_type, new_data_T>;

    explicit basic_valued_argument(argument_ptr_type argument_ptr)
        : base_type(argument_ptr)
    {
        // noop
    }

    template <typename handler_ptr_T>
    explicit basic_valued_argument(argument_ptr_type argument_ptr, handler_ptr_T handler_ptr)
        : base_type(argument_ptr, handler_ptr)
    {
        // noop
    }

    basic_valued_argument& set_default_values(const string_vector_type& values)
    {
        this->get_argument().set_default_values(values);
        return this->cast_this_to_derived();
    }

    basic_valued_argument& set_default_value(const string_type& value)
    {
        this->get_argument().set_default_value(value);
        return this->cast_this_to_derived();
    }

    basic_valued_argument& set_allowed_values(const string_vector_type& values)
    {
        this->get_argument().set_allowed_values(values);
        return this->cast_this_to_derived();
    }

    basic_valued_argument& set_value_name(const string_type& name)
    {
        this->get_argument().set_value_name(name);
        return this->cast_this_to_derived();
    }

    basic_valued_argument& set_min_count(std::size_t count)
    {
        this->get_argument().set_min_count(count);
        return this->cast_this_to_derived();
    }

    basic_valued_argument& set_max_count(std::size_t count)
    {
        this->get_argument().set_max_count(count);
        return this->cast_this_to_derived();
    }

    basic_valued_argument& set_max_count_unlimited()
    {
        this->get_argument().set_max_count_unlimited();
        return this->cast_this_to_derived();
    }

    template <typename new_data_T, typename base_type::template enable_if_converter_exist<new_data_T>::type* = nullptr>
    casted_argument_type<new_data_T> set_type()
    {
        return this->template set_type_internal<casted_argument_type<new_data_T>>(
            typename base_type::template casted_converter_type<new_data_T>());
    }

    template <typename new_data_T,
        typename base_type::template enable_if_converter_not_exist<new_data_T>::type* = nullptr>
    casted_argument_type<new_data_T> set_type()
    {
        return this->template set_type_internal<casted_argument_type<new_data_T>>();
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_VALUED_ARGUMENT_HPP_
