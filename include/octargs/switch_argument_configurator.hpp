#ifndef OCTARGS_SWITCH_ARGUMENT_CONFIGURATOR_HPP_
#define OCTARGS_SWITCH_ARGUMENT_CONFIGURATOR_HPP_

#include "argument_configurator.hpp"

#include "internal/switch_argument.hpp"

namespace oct
{
namespace args
{

template <typename char_T, typename values_storage_T, typename data_T = void>
class basic_switch_argument_configurator
    : public basic_argument_configurator<basic_switch_argument_configurator<char_T, values_storage_T, data_T>,
          internal::basic_switch_argument<char_T, values_storage_T>, char_T, values_storage_T, data_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;
    using data_type = data_T;

    using base_type = basic_argument_configurator<basic_switch_argument_configurator<char_T, values_storage_T, data_T>,
        internal::basic_switch_argument<char_T, values_storage_T>, char_T, values_storage_T, data_T>;

    using argument_ptr_type = typename base_type::argument_ptr_type;

    explicit basic_switch_argument_configurator(argument_ptr_type argument_ptr)
        : base_type(argument_ptr)
    {
        // noop
    }

    basic_switch_argument_configurator& set_min_count(std::size_t count)
    {
        this->get_argument().set_min_count(count);
        return this->cast_this_to_derived();
    }

    basic_switch_argument_configurator& set_max_count(std::size_t count)
    {
        this->get_argument().set_max_count(count);
        return this->cast_this_to_derived();
    }

    basic_switch_argument_configurator& set_max_count_unlimited()
    {
        this->get_argument().set_max_count_unlimited();
        return this->cast_this_to_derived();
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_SWITCH_ARGUMENT_CONFIGURATOR_HPP_
