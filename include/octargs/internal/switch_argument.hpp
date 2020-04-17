#ifndef OCTARGS_SWITCH_ARGUMENT_HPP_
#define OCTARGS_SWITCH_ARGUMENT_HPP_

#include "argument_base.hpp"

namespace oct
{
namespace args
{
namespace internal
{

/// \brief Switch argument
///
/// Switch arguments are arguments that does not have value, they are mainly to enable
/// or disable a feature. Example could be "--verbose" or "--dry-run".
///
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of class uses as a storage for parsed values
template <typename char_T, typename values_storage_T = null_values_storage>
class basic_switch_argument
    : public basic_argument_base<basic_switch_argument<char_T, values_storage_T>, char_T, values_storage_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using base_type
        = basic_argument_base<basic_switch_argument<char_type, values_storage_type>, char_type, values_storage_type>;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    explicit basic_switch_argument(const string_vector_type& names)
        : base_type(base_type::FLAG_IS_ASSIGNABLE_BY_NAME, names)
    {
        // noop
    }

    basic_switch_argument& set_min_count(std::size_t count)
    {
        return base_type::set_min_count(count);
    }

    basic_switch_argument& set_max_count(std::size_t count)
    {
        return base_type::set_max_count(count);
    }

    basic_switch_argument& set_max_count_unlimited()
    {
        return base_type::set_max_count_unlimited();
    }
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_SWITCH_ARGUMENT_HPP_
