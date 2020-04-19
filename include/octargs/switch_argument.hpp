#ifndef OCTARGS_SWITCH_ARGUMENT_HPP_
#define OCTARGS_SWITCH_ARGUMENT_HPP_

#include "argument_base.hpp"
#include "internal/switch_argument_impl.hpp"

namespace oct
{
namespace args
{

/// \brief Switch argument
///
/// Switch arguments are arguments that does not have value, they are mainly to enable
/// or disable a feature. Example could be "--verbose" or "--dry-run".
///
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of class uses as a storage for parsed values
template <typename char_T, typename values_storage_T, typename data_T = void>
class basic_switch_argument : public basic_argument_base<basic_switch_argument, internal::basic_switch_argument_impl,
                                  char_T, values_storage_T, data_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;
    using data_type = data_T;

    using base_type = basic_argument_base<oct::args::basic_switch_argument, internal::basic_switch_argument_impl,
        char_T, values_storage_T, data_T>;

    using argument_ptr_type = typename base_type::argument_ptr_type;

    explicit basic_switch_argument(const argument_ptr_type& argument_ptr)
        : base_type(argument_ptr)
    {
        // noop
    }

    template <typename handler_ptr_T>
    explicit basic_switch_argument(const argument_ptr_type& argument_ptr, const handler_ptr_T& handler_ptr)
        : base_type(argument_ptr, handler_ptr)
    {
        // noop
    }

    basic_switch_argument& set_min_count(std::size_t count)
    {
        this->get_argument().set_min_count(count);
        return this->cast_this_to_derived();
    }

    basic_switch_argument& set_max_count(std::size_t count)
    {
        this->get_argument().set_max_count(count);
        return this->cast_this_to_derived();
    }

    basic_switch_argument& set_max_count_unlimited()
    {
        this->get_argument().set_max_count_unlimited();
        return this->cast_this_to_derived();
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_SWITCH_ARGUMENT_HPP_
