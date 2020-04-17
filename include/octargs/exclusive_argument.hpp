#ifndef OCTARGS_EXCLUSIVE_ARGUMENT_HPP_
#define OCTARGS_EXCLUSIVE_ARGUMENT_HPP_

#include "argument_base.hpp"
#include "internal/exclusive_argument_impl.hpp"

namespace oct
{
namespace args
{

/// \brief Exclusive argument
///
/// Exclusive arguments are switch arguments (arguments without value) that could
/// be only used alone when specified in input. For example --version or --help.
///
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of class uses as a storage for parsed values
template <typename char_T, typename values_storage_T, typename data_T = void>
class basic_exclusive_argument
    : public basic_argument_base<basic_exclusive_argument<char_T, values_storage_T, data_T>,
          internal::basic_exclusive_argument_impl<char_T, values_storage_T>, char_T, values_storage_T, data_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;
    using data_type = data_T;

    using base_type = basic_argument_base<basic_exclusive_argument<char_T, values_storage_T, data_T>,
        internal::basic_exclusive_argument_impl<char_T, values_storage_T>, char_T, values_storage_T, data_T>;

    using argument_ptr_type = typename base_type::argument_ptr_type;

    explicit basic_exclusive_argument(argument_ptr_type argument_ptr)
        : base_type(argument_ptr)
    {
        // noop
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_EXCLUSIVE_ARGUMENT_HPP_
