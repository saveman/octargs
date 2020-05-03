#ifndef OCTARGS_EXCLUSIVE_ARGUMENT_IMPL_HPP_
#define OCTARGS_EXCLUSIVE_ARGUMENT_IMPL_HPP_

#include "argument_base_impl.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T, typename values_storage_T>
class basic_exclusive_argument_impl
    : public basic_argument_base_impl<basic_exclusive_argument_impl<char_T, values_storage_T>, char_T, values_storage_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using base_type = basic_argument_base_impl<basic_exclusive_argument_impl<char_type, values_storage_type>, char_type,
        values_storage_type>;

    using dictionary_type = dictionary<char_type>;
    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using parser_data_weak_ptr_type = typename base_type::parser_data_weak_ptr_type;

    explicit basic_exclusive_argument_impl(parser_data_weak_ptr_type parser_data_ptr, const string_vector_type& names)
        : base_type(parser_data_ptr, base_type::FLAG_IS_EXCLUSIVE | base_type::FLAG_IS_ASSIGNABLE_BY_NAME, names)
    {
        // noop
    }
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_EXCLUSIVE_ARGUMENT_IMPL_HPP_
