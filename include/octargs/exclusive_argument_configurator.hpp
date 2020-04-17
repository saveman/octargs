#ifndef OCTARGS_EXCLUSIVE_ARGUMENT_CONFIGURATOR_HPP_
#define OCTARGS_EXCLUSIVE_ARGUMENT_CONFIGURATOR_HPP_

#include "argument_configurator.hpp"

#include "internal/exclusive_argument.hpp"

namespace oct
{
namespace args
{

template <typename char_T, typename values_storage_T, typename data_T = void>
class basic_exclusive_argument_configurator
    : public basic_argument_configurator<basic_exclusive_argument_configurator<char_T, values_storage_T, data_T>,
          internal::basic_exclusive_argument<char_T, values_storage_T>, char_T, values_storage_T, data_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;
    using data_type = data_T;

    using base_type
        = basic_argument_configurator<basic_exclusive_argument_configurator<char_T, values_storage_T, data_T>,
            internal::basic_exclusive_argument<char_T, values_storage_T>, char_T, values_storage_T, data_T>;

    using argument_ptr_type = typename base_type::argument_ptr_type;

    explicit basic_exclusive_argument_configurator(argument_ptr_type argument_ptr)
        : base_type(argument_ptr)
    {
        // noop
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_EXCLUSIVE_ARGUMENT_CONFIGURATOR_HPP_
